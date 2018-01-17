module Create =
       (Config: {type state; type fields; let handleChange: ((fields, string), state) => state;}) => {
  /* TODO: Make a variant out of this */
  type value = string;
  /* Form actions */
  type action =
    | HandleSubmitting(bool)
    | HandleFieldValidation((Config.fields, value))
    | HandleError(option(string))
    | HandleChange((Config.fields, value))
    | HandleSubmit;
  type values = Config.state;
  /* Validation types */
  type validation =
    | Required
    | Email
    | Custom(values => option(string));
  type schema = list((Config.fields, validation));
  let validateFieldChange: (Config.fields, values, value, schema) => option(string) =
    (field, values, value, schema) => {
      let fieldValidation =
        schema |> List.filter(((fieldName, _)) => compare(fieldName, field) == 0) |> List.hd;
      switch fieldValidation {
      | (_, Required) => String.length(value) < 1 ? Some("Field is required") : None
      | (_, Custom(fn)) => fn(values)
      | _ => None
      }
    };
  type state = {
    values,
    isSubmitting: bool,
    errors: list((Config.fields, option(string))),
    error: option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make =
      (
        ~onSubmit:
           (
             values,
             ~setSubmitting: ReasonReact.Callback.t(bool),
             ~setError: ReasonReact.Callback.t(option(string))
           ) =>
           unit,
        ~validate: values => option(string),
        ~initialState: Config.state,
        ~schema: schema,
        children
      ) => {
    ...component,
    initialState: () => {values: initialState, error: None, isSubmitting: false, errors: []},
    reducer: (action, state) =>
      switch action {
      | HandleSubmitting(isSubmitting) => ReasonReact.Update({...state, isSubmitting})
      | HandleError(error) => ReasonReact.Update({...state, isSubmitting: false, error})
      | HandleFieldValidation((field, value)) =>
        ReasonReact.Update({
          ...state,
          errors:
            state.errors
            |> List.filter(((fieldName, _)) => compare(fieldName, field) !== 0)
            |> List.append([(field, validateFieldChange(field, state.values, value, schema))])
        })
      | HandleChange((field, value)) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, values: Config.handleChange((field, value), state.values)},
          ((self) => self.reduce((_) => HandleFieldValidation((field, value)), ()))
        )
      | HandleSubmit =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting: true},
          (
            self =>
              onSubmit(
                state.values,
                ~setSubmitting=
                  isSubmitting => self.send(HandleSubmitting(isSubmitting)),
                ~setError=error => self.send(HandleError(error))
              )
          )
        )
      },
    render: self => {
      let handleChange = (field, value) =>
        self.send(HandleChange((field, value)));
      let handleValidation = error => self.send(HandleError(error));
      let handleFormSubmit = (_) => self.send(HandleSubmit);
      let handleSubmit = (_) => {
        let validationError = validate(self.state.values);
        handleValidation(validationError);
        validationError == None ? handleFormSubmit() : ignore();
      };
      let getErrorForField: Config.fields => option(string) = (field) => (
        self.state.errors
        |> List.filter(((fieldName, _)) => compare(fieldName, field) == 0)
        |> List.map(((_, error)) => error)
        |> List.hd
      ); 
      children(~form=self.state, ~handleChange, ~handleSubmit, ~handleValidation, ~getErrorForField)
    }
  };
};
