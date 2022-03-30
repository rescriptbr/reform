open Ancestor.Default

let input = Emotion.css({
  "backgroundColor": `#f6f6f6`,
  "color": `var(--ifm-heading-font-color)`,
  "fontFamily": "DM Sans",
  "outline": "none",
  "border": "none",
  "width": "100%",
  "borderRadius": 8,
  "fontSize": 16,
  "padding": 16,
})

@react.component
let make = (~value, ~onChange, ~name=?, ~type_="text", ~placeholder=?) => {
  <Base tag=#input value onChange ?name ?placeholder type_ className=input />
}

module Error = {
  @react.component
  let make = (~children) =>
    <Typography m=[xs(0)] color=[xs(#hex("#f36"))] tag=#p fontSize=[xs(12->#px)]>
      {children->React.string}
    </Typography>
}
