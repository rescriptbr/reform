open Render

module Styles = {
  open CssJs
  let wrapper = style(.[display(#flex), flexDirection(#column), height(100.0->pct)])

  let content = style(.[
    flexGrow(3.0),
    backgroundColor("191818"->hex),
    selector(
      "> div",
      [height(100.0->pct), maxWidth(Theme.Constants.maxWidth), margin2(~v=zero, ~h=auto)],
    ),
  ])

  let footer = style(.[
    padding2(~v=1.2->rem, ~h=zero),
    display(#flex),
    justifyContent(#center),
    backgroundColor(Theme.Colors.black),
    selector("> p", [fontSize(1.6->rem), color(white), fontWeight(700->#num)]),
  ])
}

@react.component
let make = (~children) => {
  <div className={Styles.wrapper}> children </div>
}

module Content = {
  @react.component
  let make = (~children) => {
    <div className={Styles.content}> <div> children </div> </div>
  }
}

module Footer = {
  @react.component
  let make = () =>
    <footer className={Styles.footer}>
      <p> {str(j`Made with ReForm and ReScript ❤️  `)} </p>
    </footer>
}
