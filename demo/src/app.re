[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

module SignUpParams = {
  type state = {
    email: string,
    confirmPassword: string,
    password: string
  };
  type fields = [ | `email | `password | `confirmPassword];
  /* (fieldName, getter, setter) */
  let lens = [
    (`email, s => s.email, (s, email) => {...s, email}),
    (`password, s => s.password, (s, password) => {...s, password}),
    (
      `confirmPassword,
      s => s.confirmPassword,
      (s, confirmPassword) => {...s, confirmPassword}
    )
  ];
};

let defaults = (value, maybe) =>
  switch maybe {
  | Some(original) => original
  | None => value
  };

module SignUpForm = ReForm.Create(SignUpParams);

/* A pattern we are using to ease the reading of function as children */
let enhancer = mapper =>
  <SignUpForm
    onSubmit=((values, ~setSubmitting as _, ~setError as _) => Js.log(values))
    initialState={email: "", password: "", confirmPassword: ""}
    schema=[
      (`email, Email),
      (`password, Required),
      (
        `confirmPassword,
        Custom(
          s =>
            s.confirmPassword !== s.password ?
              Some("Passwords don't match") : None
        )
      )
    ]>
    ...mapper
  </SignUpForm>;

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    enhancer(({form, handleChange, handleSubmit, getErrorForField}) =>
      <div className="App">
        <div className="App-header">
          <img
            src="https://astrocoders.com/images/hero/logo.png"
            className="App-logo"
            alt="logo"
          />
          <h2> (ReasonReact.stringToElement(message)) </h2>
        </div>
        <p className="App-intro">
          (ReasonReact.stringToElement("Say hello to ReForm"))
        </p>
        <form
          onSubmit=(
            event => {
              ReactEventRe.Synthetic.preventDefault(event);
              handleSubmit();
            }
          )>
          <label>
            <span> ("Email:" |> ReasonReact.stringToElement) </span>
            <input
              value=form.values.email
              onChange=(
                ReForm.Helpers.handleDomFormChange(handleChange(`email))
              )
            />
            <p>
              (
                getErrorForField(`email)
                |> defaults("")
                |> ReasonReact.stringToElement
              )
            </p>
          </label>
          <label>
            <span> ("Password:" |> ReasonReact.stringToElement) </span>
            <input
              _type="password"
              value=form.values.password
              onChange=(
                ReForm.Helpers.handleDomFormChange(handleChange(`password))
              )
            />
            <p>
              (
                getErrorForField(`password)
                |> defaults("")
                |> ReasonReact.stringToElement
              )
            </p>
          </label>
          <label>
            <span> ("Confirm password:" |> ReasonReact.stringToElement) </span>
            <input
              _type="password"
              value=form.values.confirmPassword
              onChange=(
                ReForm.Helpers.handleDomFormChange(
                  handleChange(`confirmPassword)
                )
              )
            />
            <p>
              (
                getErrorForField(`confirmPassword)
                |> defaults("")
                |> ReasonReact.stringToElement
              )
            </p>
          </label>
          <button _type="submit">
            ("Submit" |> ReasonReact.stringToElement)
          </button>
        </form>
      </div>
    )
};
