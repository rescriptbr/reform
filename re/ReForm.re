type action = [ | `handleError(option(string)) | `handleChange(string, string) | `handleSubmit];

module Create =
       (Config: {type state; let handleChange: (action, state) => state; let initialState: state;}) => {
  type values = Config.state;
  type state = {
    values,
    error: option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make = (~onSubmit: values => unit, ~validate: values => option(string), children) => {
    ...component,
    initialState: () => {values: Config.initialState, error: None},
    reducer: (action, state) =>
      switch action {
      | `handleError(error) => ReasonReact.Update({...state, error})
      | `handleChange(_, _) =>
        ReasonReact.Update({...state, values: Config.handleChange(action, state.values)})
      | `handleSubmit =>
        onSubmit(state.values);
        ReasonReact.NoUpdate
      },
    render: (self) => {
      let handleChange = (field) => self.reduce((value) => `handleChange((field, value)));
      let handleValidation = self.reduce((error) => `handleError(error));
      let handleFormSubmit = self.reduce((_) => `handleSubmit);
      let handleSubmit = (_) => {
        let validationError = validate(self.state.values);
        handleValidation(validationError);
        validationError == None ? handleFormSubmit() : ignore()
      };
      children[0](~form=self.state, ~handleChange, ~handleSubmit)
    }
  };
};
