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
    Field.Make({
      type fields = Config.fields;
      type state = Config.state;
      type schema = list((Config.fields, Validation.validation(values)));
      let lens = Config.lens;
    });
  /* Form actions */
  type action =
    | HandleSubmitting(bool)
    | SetFieldStates(list((Config.fields, Field.state)))
    | UpdateFieldState((Config.fields, Field.state))
    | HandleFieldValidation((Config.fields, value))
    | HandleError(option(string))
    | HandleChange((Config.fields, value))
    | TrySubmit
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
  type api = {
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
        )
      | TrySubmit =>
        /* Before submitting the form we need to know if all fields were validated first and in sequence check if they are valid */
        Js.Promise.(
          SideEffects(
            (
              self =>
                all(
                  /* Here we'll loop through all fields using their stored states and then fetch their getter lenses and
                   ** attempt to get their new state using `Field.getNewFieldState`
                   */
                  self.state.fieldStates
                  |> List.map(((fieldName, fieldState)) =>
                       switch (fieldState) {
                       /* If the field is `Pristine` means that it was not changed by any events, so we validate now */
                       | Field.Pristine =>
                         let (_, getter, _) = Field.getFieldLens(fieldName);
                         Field.getNewFieldState(
                           fieldName,
                           state.values,
                           getter(self.state.values),
                           schema,
                           i18n,
                         )
                         |. Belt.Option.map(
                              then_(newFieldState =>
                                resolve((fieldName, newFieldState))
                              ),
                            )
                         |. Belt.Option.getWithDefault(
                              resolve((fieldName, Field.Pristine)),
                            );
                       /* If it is on any other state we'll let the current field state for the next state */
                       | _ => resolve((fieldName, fieldState))
                       }
                     )
                  |> Array.of_list,
                )
                |> then_(newFieldStates => {
                     self.send(
                       SetFieldStates(Array.to_list(newFieldStates)),
                     );
                     newFieldStates
                     |> Array.to_list
                     |. Belt.List.every(((_, fieldState)) =>
                          fieldState == Field.Valid
                        )
                     |> (
                       fun
                       | true => self.send(HandleSubmit)
                       | false => ()
                     )
                     |> resolve;
                   })
                |> ignore
            ),
          )
        )
      },
    render: self => {
      let handleChange = (field, value) =>
        self.send(HandleChange((field, value)));
      let handleGlobalValidation = error => self.send(HandleError(error));
      let handleFormSubmit = _ => self.send(TrySubmit);
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
