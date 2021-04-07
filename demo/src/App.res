open Render

@react.component
let make = () => {
  let url = RescriptReactRouter.useUrl()

  <div className=GlobalStyles.wrapper>
    <header>
      <nav className="navbar" role="navigation" ariaLabel="main navigation">
        <div className="navbar-brand">
          <a className="navbar-item" href="https://bulma.io">
            <img src="https://bulma.io/images/bulma-logo.png" width="113" height="28" />
          </a>
        </div>
        <div className="navbar-end">
          <div className="navbar-start">
            <a className="navbar-item"> {str("New Post")} </a>
            <a className="navbar-item"> {str("Todo Form")} </a>
          </div>
        </div>
      </nav>
    </header>
    {switch url.path {
    | list{"new-post"} => <NewPost />
    | _ => <Home />
    }}
  </div>
}
