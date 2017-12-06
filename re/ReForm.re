module Create =
       (
         Config: {
           type state;
           type fields;
           let handleChange: ((fields, string), state) => state;
           let initialState: state;
         }
       ) => {
  /* TODO: Make a variant out of this */
  type value = string;
  type action =
    | HandleSubmitting(bool)
    | HandleError(option(string))
    | HandleChange((Config.fields, value))
    | HandleSubmit;
  type values = Config.state;
  type validation =
    | Required
    | Email
    | Custom(values => option(string));
  type fieldValidation = (Config.fields, list(validation));
  type formSchema = list(fieldValidation);
  type state = {
    values,
    isSubmitting: bool,
    error: option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let handleSchemaValidation:
    (formSchema, ReasonReact.Callback.t(option(string)), values, (Config.fields, value)) => unit =
    (schema, setError, values, (fieldName, fieldValue)) =>
      switch (List.find(((fieldValidationName, _)) => compare(fieldValidationName, fieldName) == 0, schema)) {
      | (_, validations) =>
        validations
        |> List.iter(
             (validationType) =>
               switch validationType {
               | Required when fieldValue == "" => setError(Some("Fill up all fields"))
               | Email when Js.Re.test(fieldValue, [%bs.re "/@/"]) == false =>
                 setError(Some("Invalid email"))
               | Custom(mapper) =>
                 setError(mapper(values));
                 ()
                | _ => ()
               }
           )
        |> ignore
      | exception Not_found => setError(None)
      };
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
        ~schema: formSchema,
        children
      ) => {
    ...component,
    initialState: () => {values: Config.initialState, error: None, isSubmitting: false},
    reducer: (action, state) =>
      switch action {
      | HandleSubmitting(isSubmitting) => ReasonReact.Update({...state, isSubmitting})
      | HandleError(error) => ReasonReact.Update({...state, isSubmitting: false, error})
      | HandleChange((field, value)) =>
      ReasonReact.UpdateWithSideEffects({...state, values: Config.handleChange((field, value), state.values)}, (self) => {
        handleSchemaValidation(schema, self.reduce((error) => HandleError(error)), state.values, (field, value))
      })
      | HandleSubmit =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting: true},
          (
            (self) =>
              onSubmit(
                state.values,
                ~setSubmitting=self.reduce((isSubmitting) => HandleSubmitting(isSubmitting)),
                ~setError=self.reduce((error) => HandleError(error))
              )
          )
        )
      },
    render: (self) => {
      let handleChange = (field) => self.reduce((value) => HandleChange((field, value)));
      let handleValidation = self.reduce((error) => HandleError(error));
      let handleFormSubmit = self.reduce((_) => HandleSubmit);
      let handleSubmit = (_) => {
        let validationError = validate(self.state.values);
        handleValidation(validationError);
        validationError == None ? handleFormSubmit() : ignore()
      };
      children[0](~form=self.state, ~handleChange, ~handleSubmit, ~handleValidation)
    }
  };
};
