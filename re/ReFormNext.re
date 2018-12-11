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
    type t('a) =
      | Email(Config.field(string))
      | Optional(Config.field('a))
      | Min(Config.field(int), int)
      | Custom(Config.field('a), Config.state => fieldState);
    type schema =
      | Schema(array(t('a))): schema;
  };

  let getFieldsState = (~schema: Validation.schema, ~values: Config.state) => {
    let Validation.Schema(validators) = schema;

    validators
    ->Belt.Array.map(validator =>
        switch (validator) {
        | Validation.Min(field, min) => (
            Field(field),
            Config.get(values, field) > min ? Valid : Error("Below minimum"),
          )
        | Validation.Email(field) => (
            Field(field),
            Js.Re.test(
              Config.get(values, field),
              [%bs.re {|/\S+@\S+\.\S+/|}],
            ) ?
              Valid : Error("invalid email"),
          )
        | Validation.Optional(field) => (Field(field), Valid)
        | Validation.Custom(field, predicate) => (
            Field(field),
            predicate(values),
          )
        }
      );
  };

  type formState =
    | Dirty
    | Submitting
    | Pristine
    | Errored
    | Valid;

  type action =
    | ValidateAll
    | TrySubmit
    | Submit
    | SetFieldsState(array((field, fieldState)))
    | FieldChangeState(field, fieldState)
    | FieldChangeValue(Config.field('a), 'a): action
    | FieldArrayAdd(Config.field(array('a)), 'a): action
    | FieldArrayRemove(Config.field(array('a)), int): action
    | FieldArrayRemoveBy(Config.field(array('a)), 'a => bool): action
    | SetFormState(formState);

  type state = {
    formState,
    values: Config.state,
    fieldsState: array((field, fieldState)),
  };

  let component = ReasonReact.reducerComponent("ReForm");

  let make = (~initialState, ~schema: Validation.schema, ~onSubmit, children) => {
    ...component,
    initialState: () => {
      fieldsState: getFieldsState(~schema, ~values=initialState),
      values: initialState,
      formState: Pristine,
    },
    reducer: (action, state) =>
      switch (action) {
      | Submit =>
        UpdateWithSideEffects(
          {...state, formState: Submitting},
          (self => onSubmit(~send=self.send, ~state=self.state)),
        )
      | TrySubmit =>
        SideEffects(
          (
            self => {
              let fieldsState =
                getFieldsState(~schema, ~values=self.state.values);

              self.send(SetFieldsState(fieldsState));

              if (fieldsState
                  ->Belt.Array.every(((_, fieldState)) =>
                      fieldState == Valid
                    )) {
                self.send(SetFormState(Valid));
                self.send(Submit);
              } else {
                self.send(SetFormState(Errored));
              };
            }
          ),
        )
      | SetFieldsState(fieldsState) => Update({...state, fieldsState})
      | ValidateAll =>
        Update({
          ...state,
          fieldsState: getFieldsState(~schema, ~values=state.values),
        })
      | FieldChangeValue(field, value) =>
        UpdateWithSideEffects(
          {
            ...state,
            formState: state.formState == Errored ? Errored : Dirty,
            values: Config.set(state.values, field, value),
          },
          (self => self.send(ValidateAll)),
        )
      | FieldChangeState(_, _) => NoUpdate
      | FieldArrayAdd(field, entry) =>
        Update({
          ...state,
          values:
            Config.set(
              state.values,
              field,
              Belt.Array.concat([|entry|], Config.get(state.values, field)),
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
      | SetFormState(newState) => Update({...state, formState: newState})
      },
    render: self => children(~send=self.send, ~state=self.state),
  };
};
