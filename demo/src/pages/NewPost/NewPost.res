open Render

@react.component
let make = () => {
  <form className="box">
    <div className="field">
      <label className="label"> {str("Email")} </label>
      <div className="control">
        <input className="input" type_="email" placeholder="e.g. alex@example.com" />
      </div>
    </div>
    <div className="field">
      <label className="label"> {str("Password")} </label>
      <div className="control">
        <input className="input" type_="password" placeholder="********" />
      </div>
    </div>
    <button className="button is-primary"> {str("Sign in")} </button>
  </form>
}
