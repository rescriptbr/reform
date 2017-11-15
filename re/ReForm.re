type action =
  | HandleChange((string, string))
  | HandleSubmit;

module Create =
       (Config: {type state; let handleChange: (action, state) => state; let initialState: state;}) => {
  type values = Config.state;
  type state = {
    values,
    errors: Config.state
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make = (~onSubmit: values => unit, children) => {
    ...component,
    initialState: () => {values: Config.initialState, errors: Config.initialState},
    reducer: (action, state) =>
      switch action {
      | HandleChange((_, _)) =>
        ReasonReact.Update({...state, values: Config.handleChange(action, state.values)})
      | HandleSubmit =>
        onSubmit(state.values);
        ReasonReact.NoUpdate
      },
    render: (self) => {
      let handleChange = (field) => self.reduce((value) => HandleChange((field, value)));
      let handleSubmit = self.reduce((_) => HandleSubmit);
      children[0](~form=self.state, ~handleChange, ~handleSubmit)
    }
  };
};
