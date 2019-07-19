module SignUpFormParams = [%lenses
  /* Define the form state */
  type state = {
    password: string,
    confirm: string,
    confirmPassword: bool,
    email: string,
  }
];
module SignUpFormContainer = ReFormNext.Make(SignUpFormParams);

[@react.component]
let make = () => {
  let {state, submit, getFieldState, handleChange}: SignUpFormContainer.api =
    SignUpFormContainer.use(
      ~schema={
        SignUpFormContainer.Validation.Schema([|
          // example of using a default validator on the Validation module
          Email(Email),
          // examples of using a custom validator on the Validation module
          Custom(
            Password,
            values => values.password == "" ? Error("Can't be empty") : Valid,
          ),
          Custom(
            Confirm,
            values => values.confirm == "" ? Error("Can't be empty") : Valid,
          ),
          Custom(
            ConfirmPassword,
            values =>
              values.password == values.confirm
                ? Error("Password do not match") : Valid,
          ),
        |]);
      },
      ~onSubmit=
        ({state}) => {
          Js.log("onSubmit");
          Js.log(state.values);
          None;
        },
      ~initialState={
        email: "",
        password: "",
        confirm: "",
        confirmPassword: false,
      },
      (),
    );
  <form
    onSubmit={event => {
      Js.log("event");
      Js.log(event);
      ReactEvent.Synthetic.preventDefault(event);
      submit();
    }}>
    <label>
      <span> {"Email:" |> React.string} </span>
      <input
        value={state.values.email}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(Email))}
      />
      <p>
        {getFieldState(Field(Email))
         |> (
           fun
           | Error(error) => Some(error)
           | _ => None
         )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </label>
    <label>
      <span> {"Password:" |> React.string} </span>
      <input
        typeof="password"
        value={state.values.password}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(Password))}
      />
      <p>
        {getFieldState(Field(Password))
         |> (
           fun
           | Error(error) => Some(error)
           | _ => None
         )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </label>
    <label>
      <span> {"Confirm password:" |> React.string} </span>
      <input
        typeof="password"
        value={state.values.confirm}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(Confirm))}
      />
      <p>
        {getFieldState(Field(Password))
         |> (
           fun
           | Error(error) => Some(error)
           | _ => None
         )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </label>
    <button typeof="submit"> {"Submit" |> React.string} </button>
  </form>;
};
