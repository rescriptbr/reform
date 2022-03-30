module Styles = {
  let button = Emotion.css({
    "cursor": "pointer",
    "background": `var(--red)`,
    "borderRadius": 8,
    "width": "100%",
    "color": `#fff`,
    "fontFamily": `DM Sans`,
    "padding": 16,
    "fontWeight": 700,
    "border": 0,
    "fontSize": 18,
    "&:hover": {
      "backgroundColor": `var(--ifm-color-primary-dark)`,
    },
  })
}
@react.component
let make = (~children, ~onClick=?) => <button ?onClick className=Styles.button> children </button>
