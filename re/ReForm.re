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
  type state = {
    values,
    isSubmitting: bool,
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
        children
      ) => {
    ...component,
    initialState: () => {values: Config.initialState, error: None, isSubmitting: false},
    reducer: (action, state) =>
      switch action {
      | HandleSubmitting(isSubmitting) => ReasonReact.Update({...state, isSubmitting})
      | HandleError(error) => ReasonReact.Update({...state, isSubmitting: false, error})
      | HandleChange((field, value)) =>
        ReasonReact.Update({...state, values: Config.handleChange((field, value), state.values)})
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
