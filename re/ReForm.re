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
    | HandlePairUpdate((Config.fields, value))
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
    pairValues: list((Config.fields, value)),
    isSubmitting: bool,
    error: option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let handleSchemaValidation:
    (formSchema, ReasonReact.Callback.t(option(string)), values, (Config.fields, value)) => list(bool) =
    (schema, setError, values, (fieldName, fieldValue)) =>
      switch (
        List.find(
          ((fieldValidationName, _)) => compare(fieldValidationName, fieldName) == 0,
          schema
        )
      ) {
      | (_, validations) =>
        validations
        |> List.map(
             (validationType) =>
               switch validationType {
               | Required when fieldValue == "" => setError(Some("Fill up all fields")); true
               | Email when Js.Re.test(fieldValue, [%bs.re "/@/"]) == false =>
                 setError(Some("Invalid email"));
                 true
               | Custom(mapper) =>
                 setError(mapper(values));
                 true
               | _ => false
               }
           )
      | exception Not_found => setError(None); []
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
    initialState: () => {
      values: Config.initialState,
      error: None,
      isSubmitting: false,
      pairValues: []
    },
    reducer: (action, state) =>
      switch action {
      | HandleSubmitting(isSubmitting) => ReasonReact.Update({...state, isSubmitting})
      | HandleError(error) => ReasonReact.Update({...state, isSubmitting: false, error})
      | HandlePairUpdate((field, value)) =>
        ReasonReact.Update({
          ...state,
          pairValues:
            state.pairValues
            |> List.filter(((field', _)) => compare(field, field') != 0)
            |> List.append([(field, value)])
        })
      | HandleChange((field, value)) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, values: Config.handleChange((field, value), state.values)},
          (
            (self) =>
              handleSchemaValidation(
                schema,
                self.reduce((error) => HandleError(error)),
                state.values,
                (field, value)
              ) |> ignore
          )
        )
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
        let pairValuesError = self.state.pairValues
        |> List.map(
             ((field, value)) =>
               handleSchemaValidation(
                 schema,
                 self.reduce((error) => HandleError(error)),
                 self.state.values,
                 (field, value)
               )
           )
        |> List.flatten;
        validationError == None || List.mem(true, pairValuesError) ? handleFormSubmit() : ignore()
      };
      children[0](~form=self.state, ~handleChange, ~handleSubmit, ~handleValidation)
    }
  };
};
