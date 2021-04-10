open Render

let button_ = {
  open CssJs
  style(.[
    //
    backgroundColor(Theme.Colors.red),
    width(100.0->pct),
    height(52->px),
    color(white),
    borderStyle(none),
    outlineStyle(none),
    fontSize(1.6->rem),
    fontWeight(600->#num),
    borderRadius(4->px),
    cursor(pointer),
    transitionDuration(200),
    hover([transitionDuration(200), backgroundColor("af1140"->hex)]),
  ])
}

@react.component
let make = (~children, ~type_=?) => {
  <button className=button_ ?type_> {children->str} </button>
}
