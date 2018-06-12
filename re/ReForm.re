module Helpers = Helpers;

module type Config = {
  type state;
  type fields;
  /* (fieldName, getter, setter) */
  let lens: list((fields, state => Value.t, (state, Value.t) => state));
};

module Create = (Config: Config) => {
  /* TODO: Make a variant out of this */
  type value = Value.t;
  type values = Config.state;
  type schema = list((Config.fields, Validation.validation(values)));
  module Field =
    Field.Make(
      {
        type fields = Config.fields;
        type state = Config.state;
        type schema = list((Config.fields, Validation.validation(values)));
        let lens = Config.lens;
      },
    );
  /* Form actions */
  type action =
    | HandleSubmitting(bool)
    | SetFieldStates(list((Config.fields, Field.state)))
    | UpdateFieldState((Config.fields, Field.state))
    | HandleFieldValidation((Config.fields, value))
    | HandleError(option(string))
    | HandleChange((Config.fields, value))
    | HandleSubmit
    | ResetFormState;
  type onSubmit = {
    values,
    setSubmitting: bool => unit,
    setError: option(string) => unit,
    resetFormState: unit => unit,
  };
  type state = {
    values,
    isSubmitting: bool,
    fieldStates: list((Config.fields, Field.state)),
    error: option(string),
  };
  /* Type of what is given to the children */
  type reform = {
    form: state,
    handleChange: (Config.fields, value) => unit,
    handleGlobalValidation: option(string) => unit,
    handleSubmit: unit => unit,
    getFieldState: Config.fields => Field.state,
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make =
      (
        ~onSubmit: onSubmit => unit,
        ~onFormStateChange: state => unit=ignore,
        ~validate: values => option(string)=(_) => None,
        ~initialState: Config.state,
        ~schema: schema,
        ~i18n: Validation.I18n.dictionary=Validation.I18n.en,
        children,
      ) => {
    ...component,
    initialState: () => {
      values: initialState,
      error: None,
      isSubmitting: false,
      fieldStates:
        Config.lens |> List.map(((field, _, _)) => (field, Field.Pristine)),
    },
    reducer: (action, state) =>
      switch (action) {
      | ResetFormState =>
        UpdateWithSideEffects(
          {
            ...state,
            values: initialState,
            fieldStates:
              Config.lens
              |> List.map(((field, _, _)) => (field, Field.Pristine)),
            isSubmitting: false,
          },
          (self => onFormStateChange(self.state)),
        )
      | HandleSubmitting(isSubmitting) =>
        UpdateWithSideEffects(
          {...state, isSubmitting},
          (self => onFormStateChange(self.state)),
        )
      | HandleError(error) =>
        UpdateWithSideEffects(
          {...state, isSubmitting: false, error},
          (self => onFormStateChange(self.state)),
        )
      | SetFieldStates(fieldStates) =>
        UpdateWithSideEffects(
          {...state, isSubmitting: false, fieldStates},
          (self => onFormStateChange(self.state)),
        )
      | UpdateFieldState((field, newFieldState)) =>
        UpdateWithSideEffects(
          {
            ...state,
            fieldStates:
              state.fieldStates
              |> List.filter(((fieldName, _)) => fieldName !== field)
              |> List.append([(field, newFieldState)]),
          },
          (self => onFormStateChange(self.state)),
        )
      | HandleFieldValidation((field, value)) =>
        SideEffects(
          (
            self => {
              self.send(UpdateFieldState((field, Field.Validating)));
              Js.Promise.(
                Field.getNewFieldState(
                  field,
                  state.values,
                  value,
                  schema,
                  i18n,
                )
                |. Belt.Option.map(
                     then_(error =>
                       self.send(UpdateFieldState((field, error))) |> resolve
                     ),
                   )
                |> ignore
              );
            }
          ),
        )
      | HandleChange((field, value)) =>
        UpdateWithSideEffects(
          {
            ...state,
            values: Field.handleChange((field, value), state.values),
          },
          (self => self.send(HandleFieldValidation((field, value)))),
        )
      | HandleSubmit =>
        state.fieldStates
        |. Belt.List.every(((_, fieldState)) => fieldState == Valid) ?
          UpdateWithSideEffects(
            {...state, isSubmitting: true},
            (
              self => {
                onSubmit({
                  resetFormState: () => self.send(ResetFormState),
                  values: state.values,
                  setSubmitting: isSubmitting =>
                    self.send(HandleSubmitting(isSubmitting)),
                  setError: error => self.send(HandleError(error)),
                });
                onFormStateChange(self.state);
              }
            ),
          ) :
          NoUpdate
      },
    render: self => {
      let handleChange = (field, value) =>
        self.send(HandleChange((field, value)));
      let handleGlobalValidation = error => self.send(HandleError(error));
      let handleFormSubmit = (_) => self.send(HandleSubmit);
      let getFieldState = field =>
        self.state.fieldStates
        |> List.filter(((fieldName, _)) => fieldName === field)
        |> List.map(((_, error)) => error)
        |. Belt.List.head
        |. Belt.Option.getWithDefault(Field.Pristine);
      children({
        form: self.state,
        handleChange,
        handleSubmit: handleFormSubmit,
        handleGlobalValidation,
        getFieldState,
      });
    },
  };
};
