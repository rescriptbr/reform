type action =
  | HandleError(option(string))
  | HandleChange((string, string))
  | HandleSubmit;

module Create =
       (Config: {type state; let handleChange: (action, state) => state; let initialState: state;}) => {
  type values = Config.state;
  type state = {
    values,
    isSubmitting: bool,
    error: option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make = (~onSubmit: values => unit, ~validate: values => option(string), children) => {
    ...component,
    initialState: () => {values: Config.initialState, error: None, isSubmitting: false},
    reducer: (action, state) =>
      switch action {
      | HandleError(error) => ReasonReact.Update({ ...state, isSubmitting: false, error })
      | HandleChange((_, _)) =>
        ReasonReact.Update({...state, values: Config.handleChange(action, state.values)})
      | HandleSubmit =>
        ReasonReact.UpdateWithSideEffects({...state, isSubmitting: true}, (_) => onSubmit(state.values))
      },
    render: (self) => {
      let handleChange = (field) => self.reduce((value) => HandleChange((field, value)));
      let handleValidation = self.reduce(error => HandleError(error));
      let handleFormSubmit = self.reduce((_) => HandleSubmit);
      let handleSubmit = (_) => {
        let validationError = validate(self.state.values);
        handleValidation(validationError);
        validationError == None ? handleFormSubmit() : ignore();
      };
      children[0](~form=self.state, ~handleChange, ~handleSubmit)
    }
  };
};
