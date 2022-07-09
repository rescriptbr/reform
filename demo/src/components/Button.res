open Ancestor

let button = Emotion.css({
  "fontWeight": 600,
  "background": "#000",
  "width": "100%",
  "padding": `1.2rem 1.2rem`,
  "color": "#fafafa",
  "borderRadius": 4,
  "border": 0,
  "outline": "none",
  "fontFamily": "Cairo",
  "fontSize": 20,
  "display": "flex",
  "alignItems": "center",
  "justifyContent": "center",
  "willChange": "background-color",
  "transition": "200ms background-color",
  "cursor": "pointer",
  "&:hover": {
    "transition": "200ms background-color",
    "backgroundColor": "rgba(0,0,0,0.8)",
  },
})

@react.component
let make = (~onClick=?, ~label) => {
  <Base tag=#button className=button ?onClick> {label->React.string} </Base>
}
