module type Config = {
  type field('a);
  type state;
  let set: (state, field('a), 'a) => state;
  let get: (state, field('a)) => 'a;
};
/* This is the abstraction, the user won't know about it */
module Make = (Config: Config) => {
  type field =
    | Field(Config.field('a)): field;
  type fieldState =
    | Pristine
    | Valid
    | Error(string);

  module Validation = {
    type t =
      | Email(Config.field(string)): t
      | Optional(Config.field('a)): t
      | Min(Config.field(int), int): t
      | Custom(Config.field('a), Config.state => fieldState): t;
    type schema =
      | Schema(array(t)): schema;
  };

  let filterFieldsStateByField = (~validators, ~fieldFilter) =>
    validators->Belt.Array.keep(validator =>
      switch (validator) {
      | Validation.Min(field, _) => Field(field) == fieldFilter
      | Validation.Email(field) => Field(field) == fieldFilter
      | Validation.Optional(field) => Field(field) == fieldFilter
      | Validation.Custom(field, _) => Field(field) == fieldFilter
      }
    );

  let validateField = (~validator, ~values) =>
    switch (validator) {
    | Validation.Min(field, min) => (
        Field(field),
        Config.get(values, field) > min ? Valid : Error("Below minimum"),
      )
    | Validation.Email(field) => (
        Field(field),
        Js.Re.test(
          Config.get(values, field),
          [%bs.re
            "/^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/"
          ],
        )
          ? Valid : Error("Invalid email"),
      )
    | Validation.Optional(field) => (Field(field), Valid)
    | Validation.Custom(field, predicate) => (
        Field(field),
        predicate(values),
      )
    };

  let getInitialFieldsState = (~schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    validators->Belt.Array.map(validator =>
      switch (validator) {
      | Validation.Min(field, _min) => (Field(field), Pristine)
      | Validation.Email(field) => (Field(field), Pristine)
      | Validation.Optional(field) => (Field(field), Pristine)
      | Validation.Custom(field, _predicate) => (Field(field), Pristine)
      }
    );
  };

  let getFieldsState = (~schema: Validation.schema, ~values: Config.state) => {
    let Validation.Schema(validators) = schema;

    validators->Belt.Array.map(validator =>
      validateField(~validator, ~values)
    );
  };

  let getFieldState =
      (~schema: Validation.schema, ~values: Config.state, ~field) => {
    let Validation.Schema(validators) = schema;

    filterFieldsStateByField(~validators, ~fieldFilter=field)
    ->Belt.Array.map(validator => validateField(~validator, ~values))
    ->Belt.Array.get(0);
  };

  type formState =
    | Dirty
    | Submitting
    | Pristine
    | Errored
    | Valid;

  type action =
    | ValidateAll
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
    | SetFormState(formState);

  type state = {
    formState,
    values: Config.state,
    fieldsState: array((field, fieldState)),
  };

  type api = {
    state,
    getFieldState: field => fieldState,
    handleChange: 'a. (Config.field('a), 'a) => unit,
    arrayPush: 'a. (Config.field(array('a)), 'a) => unit,
    arrayUpdateByIndex:
      'a.
      (~field: Config.field(array('a)), ~index: int, 'a) => unit,

    arrayRemoveByIndex: 'a. (Config.field(array('a)), int) => unit,
    arrayRemoveBy: 'a. (Config.field(array('a)), 'a => bool) => unit,
    submit: unit => unit,
  };
  type onSubmitAPI = {
    send: action => unit,
    state,
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
          fieldsState: getInitialFieldsState(~schema),
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
              let fieldsState =
                getFieldsState(~schema, ~values=self.state.values);

              self.send(SetFieldsState(fieldsState));

              if (fieldsState->Belt.Array.every(((_, fieldState)) =>
                    fieldState == Valid
                  )) {
                self.send(SetFormState(Valid));
                self.send(Submit);
              } else {
                onSubmitFail({send: self.send, state: self.state});
                self.send(SetFormState(Errored));
              };
              None;
            },
          )
        | SetFieldsState(fieldsState) => Update({...state, fieldsState})
        | ValidateField(field) =>
          let fieldValidated =
            getFieldState(~schema, ~values=state.values, ~field);
          Update({
            ...state,
            fieldsState:
              state.fieldsState
              ->Belt.Array.keep(elem =>
                  elem
                  |> (((fieldValue, _fieldState)) => fieldValue != field)
                )
              ->Belt.Array.concat(
                  switch (fieldValidated) {
                  | Some(fieldState) => [|fieldState|]
                  | None => [||]
                  },
                ),
            formState: Dirty,
          });
        | ValidateAll =>
          let fieldsState = getFieldsState(~schema, ~values=state.values);
          Update({
            ...state,
            fieldsState,
            formState:
              fieldsState->Belt.Array.some(((_, fieldState)) =>
                fieldState
                |> (
                  fun
                  | Error(_) => true
                  | _ => false
                )
              )
                ? Errored : Valid,
          });
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

    let interface: api = {
      state,
      submit: () => send(TrySubmit),
      getFieldState,
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
