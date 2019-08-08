module type Config = {
  type field('a);
  type state;
  let set: (state, field('a), 'a) => state;
  let get: (state, field('a)) => 'a;
};
type fieldState =
  | Pristine
  | Valid
  | Error(string);
module Make = (Config: Config) => {
  module ReSchema = ReSchema.Make(Config);
  module Validation = ReSchema.Validation;

  type field = ReSchema.field;

  type formState =
    | Dirty
    | Submitting
    | Pristine
    | Errored
    | Valid;

  type action =
    | ValidateField(field)
    | TrySubmit
    | Submit
    | SetFieldsState(array((field, fieldState)))
    | FieldChangeState(field, fieldState)
    | FieldChangeValue(Config.field('a), 'a): action
    | FieldArrayAdd(Config.field(array('a)), 'a): action
    | FieldArrayUpdateByIndex(Config.field(array('a)), 'a, int): action
    | FieldArrayRemove(Config.field(array('a)), int): action
    | FieldArrayRemoveBy(Config.field(array('a)), 'a => bool): action
    | SetFormState(formState)
    | ResetForm
    | SetValues(Config.state)
    | SetFieldValue(Config.field('a), 'a): action;

  type state = {
    formState,
    values: Config.state,
    fieldsState: array((field, fieldState)),
  };

  type api = {
    state,
    getFieldState: field => fieldState,
    getFieldError: field => option(string),
    handleChange: 'a. (Config.field('a), 'a) => unit,
    arrayPush: 'a. (Config.field(array('a)), 'a) => unit,
    arrayUpdateByIndex:
      'a.
      (~field: Config.field(array('a)), ~index: int, 'a) => unit,

    arrayRemoveByIndex: 'a. (Config.field(array('a)), int) => unit,
    arrayRemoveBy: 'a. (Config.field(array('a)), 'a => bool) => unit,
    submit: unit => unit,
    resetForm: unit => unit,
    setValues: Config.state => unit,
    setFieldValue:
      'a.
      (Config.field('a), 'a, ~shouldValidate: bool=?, unit) => unit,

  };

  type onSubmitAPI = {
    send: action => unit,
    state,
  };

  let getInitialFieldsState: Validation.schema => array((field, fieldState)) =
    schema => {
      let Validation.Schema(validators) = schema;
      validators->Belt.Array.map(validator =>
        switch (validator) {
        | Validation.IntMin(field, _min) => (
            ReSchema.Field(field),
            Pristine: fieldState,
          )
        | Validation.IntMax(field, _max) => (Field(field), Pristine)
        | Validation.FloatMin(field, _min) => (Field(field), Pristine)
        | Validation.FloatMax(field, _max) => (Field(field), Pristine)
        | Validation.Email(field) => (Field(field), Pristine)
        | Validation.NoValidation(field) => (Field(field), Pristine)
        | Validation.StringNonEmpty(field) => (Field(field), Pristine)
        | Validation.StringRegExp(field, _regexp) => (
            Field(field),
            Pristine,
          )
        | Validation.StringMin(field, _min) => (Field(field), Pristine)
        | Validation.StringMax(field, _max) => (Field(field), Pristine)
        | Validation.Custom(field, _predicate) => (Field(field), Pristine)
        }
      );
    };

  let use =
      (
        ~initialState,
        ~schema: Validation.schema,
        ~onSubmit,
        ~onSubmitFail=ignore,
        (),
      ) => {
    let (state, send) =
      ReactUpdate.useReducer(
        {
          fieldsState: getInitialFieldsState(schema),
          values: initialState,
          formState: Pristine,
        },
        (action, state) =>
        switch (action) {
        | Submit =>
          UpdateWithSideEffects(
            {...state, formState: Submitting},
            self => onSubmit({send: self.send, state: self.state}),
          )
        | TrySubmit =>
          SideEffects(
            self => {
              let recordState =
                schema |> ReSchema.validate(self.state.values);

              switch (recordState) {
              | Valid =>
                self.send(SetFormState(Valid));
                self.send(Submit);
              | Errors(erroredFields) =>
                let newFieldsState =
                  erroredFields->Belt.Array.map(((field, errorMessage)) =>
                    (field, Error(errorMessage))
                  );
                self.send(SetFieldsState(newFieldsState));
                onSubmitFail({send: self.send, state: self.state});
                self.send(SetFormState(Errored));
              };
              None;
            },
          )
        | SetFieldsState(fieldsState) => Update({...state, fieldsState})
        | ValidateField(field) =>
          SideEffects(
            self => {
              let fieldState =
                schema
                |> ReSchema.validateOne(~field, ~values=self.state.values);
              let newFieldState: option(fieldState) =
                fieldState->Belt.Option.map(
                  fun
                  | (_, Error(message)) => Error(message)
                  | (_, Valid) => Valid,
                );

              let newFieldsState =
                state.fieldsState
                ->Belt.Array.keep(elem =>
                    elem
                    |> (((fieldValue, _fieldState)) => fieldValue != field)
                  )
                ->Belt.Array.concat(
                    switch (newFieldState) {
                    | Some(fieldState) => [|(field, fieldState)|]
                    | None => [||]
                    },
                  );
              self.send(SetFieldsState(newFieldsState));
              None;
            },
          )
        | FieldChangeValue(field, value) =>
          UpdateWithSideEffects(
            {
              ...state,
              formState: state.formState == Errored ? Errored : Dirty,
              values: Config.set(state.values, field, value),
            },
            self => {
              self.send(ValidateField(Field(field)));
              None;
            },
          )
        | FieldChangeState(_, _) => NoUpdate
        | FieldArrayAdd(field, entry) =>
          Update({
            ...state,
            values:
              Config.set(
                state.values,
                field,
                Belt.Array.concat(
                  Config.get(state.values, field),
                  [|entry|],
                ),
              ),
          })
        | FieldArrayRemove(field, index) =>
          Update({
            ...state,
            values:
              Config.set(
                state.values,
                field,
                Config.get(state.values, field)
                ->Belt.Array.keepWithIndex((_, i) => i != index),
              ),
          })
        | FieldArrayRemoveBy(field, predicate) =>
          Update({
            ...state,
            values:
              Config.set(
                state.values,
                field,
                Config.get(state.values, field)
                ->Belt.Array.keep(entry => !predicate(entry)),
              ),
          })
        | FieldArrayUpdateByIndex(field, value, index) =>
          Update({
            ...state,
            values:
              Config.set(
                state.values,
                field,
                Config.get(state.values, field)
                ->Belt.Array.mapWithIndex((i, currentValue) =>
                    i == index ? value : currentValue
                  ),
              ),
          })
        | SetFormState(newState) => Update({...state, formState: newState})
        | ResetForm =>
          Update({
            fieldsState: getInitialFieldsState(schema),
            values: initialState,
            formState: Pristine,
          })
        | SetValues(values) => Update({...state, values})
        | SetFieldValue(field, value) =>
          Update({...state, values: Config.set(state.values, field, value)})
        }
      );

    let getFieldState = field =>
      state.fieldsState
      ->Array.to_list
      ->Belt.List.getBy(((nameField, _nameFieldState)) =>
          switch (nameField == field) {
          | true => true
          | _ => false
          }
        )
      |> (
        field =>
          switch (field) {
          | Some((_nameField, nameFieldState)) => nameFieldState
          | None => Pristine
          }
      );

    let getFieldError = field =>
      getFieldState(field)
      |> (
        fun
        | Error(error) => Some(error)
        | _ => None
      );

    let interface: api = {
      state,
      submit: () => send(TrySubmit),
      resetForm: () => send(ResetForm),
      setValues: values => send(SetValues(values)),
      setFieldValue: (field, value, ~shouldValidate=true, ()) => {
        shouldValidate
          ? send(FieldChangeValue(field, value))
          : send(SetFieldValue(field, value));
      },
      getFieldState,
      getFieldError,
      handleChange: (field, value) => send(FieldChangeValue(field, value)),

      arrayPush: (field, value) => send(FieldArrayAdd(field, value)),
      arrayUpdateByIndex: (~field, ~index, value) =>
        send(FieldArrayUpdateByIndex(field, value, index)),
      arrayRemoveBy: (field, predicate) =>
        send(FieldArrayRemoveBy(field, predicate)),
      arrayRemoveByIndex: (field, index) =>
        send(FieldArrayRemove(field, index)),
    };

    interface;
  };
};
