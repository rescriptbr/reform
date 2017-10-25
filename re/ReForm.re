type action = HandleChange (string, string) | HandleSubmit;

module Create (Config: {
  type state;
  let handleChange: action => state => state;
  let initialState: state;
}) => {

  type values = Config.state;
  type state = {
    values,
    errors: Config.state
  };

  let component = ReasonReact.reducerComponent "ReForm";

  let make handleSubmit::(handleSubmit: values => unit) children => {
    ...component,
    initialState: fun () => {values: Config.initialState, errors: Config.initialState},
    reducer: fun action state => switch action {
      | HandleChange (_, _) => ReasonReact.Update {...state, values: (Config.handleChange action state.values)}
      | HandleSubmit => {
        handleSubmit(state.values);
        ReasonReact.NoUpdate
      }
    },
    render: fun self => {
      let handleChange = fun field => self.reduce (fun value => HandleChange (field, value));
      (children.(0) self.state handleChange)
    }
  };
};
