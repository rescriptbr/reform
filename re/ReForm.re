module Create =
       (Config: {type state; type fields; let handleChange: ((fields, string), state) => state;}) => {
  /* TODO: Make a variant out of this */
  type value = string;
  /* Form actions */
  type action =
    | HandleSubmitting(bool)
    | SetFieldsErrors(list((Config.fields, option(string))))
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
  type fieldGetter = (values) => value;
  type schema = list((Config.fields, fieldGetter, validation));
  let validateField: (Config.fields, values, value, schema) => option(string) =
    (field, values, value, schema) => {
      let fieldValidation =
        schema |> List.filter(((fieldName, _, _)) => fieldName === field) |> List.hd;
      switch fieldValidation {
      | (_, _, Required) => String.length(value) < 1 ? Some("Field is required") : None
      | (_, _, Custom(fn)) => fn(values)
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
        ~validate: values => option(string)=(_) => None,
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
      | SetFieldsErrors(errors) => ReasonReact.Update({...state, isSubmitting: false, errors})
      | HandleFieldValidation((field, value)) =>
        ReasonReact.Update({
          ...state,
          errors:
            state.errors
            |> List.filter(((fieldName, _)) => fieldName !== field)
            |> List.append([(field, validateField(field, state.values, value, schema))])
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
            (self) =>
              onSubmit(
                state.values,
                ~setSubmitting=(isSubmitting) => self.send(HandleSubmitting(isSubmitting)),
                ~setError=(error) => self.send(HandleError(error))
              )
          )
        )
      },
    render: (self) => {
      let handleChange = (field, value) => self.send(HandleChange((field, value)));
      let handleValidation = (error) => self.send(HandleError(error));
      let handleFormSubmit = (_) => self.send(HandleSubmit);
      let handleSubmit = (_) => {
        let globalValidationError = validate(self.state.values);
        let fieldsValidationErrors = schema
          |> List.map(((fieldName, getter, _)) => (fieldName, validateField(fieldName, self.state.values, getter(self.state.values), schema)))
          |> List.filter((( _, fieldError )) => fieldError === None);

        self.send(SetFieldsErrors(fieldsValidationErrors));

        handleValidation(globalValidationError);
        globalValidationError === None || List.length(fieldsValidationErrors) == 0 ? handleFormSubmit() : ignore()
      };
      let getErrorForField: Config.fields => option(string) =
        (field) =>
          self.state.errors
          |> List.filter(((fieldName, _)) => fieldName === field)
          |> List.map(((_, error)) => error)
          |> (finalList) => List.length(finalList) == 0 ? None : List.hd(finalList);
      children(
        ~form=self.state,
        ~handleChange,
        ~handleSubmit,
        ~handleValidation,
        ~getErrorForField
      )
    }
  };
};
