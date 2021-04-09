open Render
module Styles = Header_Styles

let links: array<(string, string)> = [
  ("basic-usage", "Basic usage"),
  ("sign-up", "Sign up"),
  ("/todo-list", "Todo List"),
]

@react.component
let make = () => {
  let url = RescriptReactRouter.useUrl()

  let goTo = (url, _) => {
    RescriptReactRouter.push(url)
  }

  let getActiveClass = path =>
    switch url.path {
    | list{current} when path === current => Styles.activeLink
    | _ => ""
    }

  <header className={Styles.header}>
    <nav className={Styles.navbar}>
      <img src={Assets.reformLogo} className={Styles.logo} />
      <ul>
        {links->map((key, (href, label)) =>
          <li key className={getActiveClass(href)}> <a onClick={goTo(href)}> {label->str} </a> </li>
        )}
      </ul>
    </nav>
  </header>
}
