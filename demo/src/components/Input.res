open Ancestor

let input = (~error) =>
  Emotion.css({
    "color": "#000",
    "fontFamily": "Cairo",
    "fontWeight": 500,
    "width": "100%",
    "height": 58,
    "fontSize": 18,
    "borderRadius": 4,
    "outline": "none",
    "fontSize": 18,
    "border": switch error {
    | None => "solid 1px #d6d6d6"
    | Some(_) => "solid 1px #ff5a5a"
    },
    "padding": `0rem 1.6rem`,
  })

@react.component
let make = (~placeholder=?, ~onChange=?, ~type_=?, ~value=?, ~error=?) => {
  <Box>
    <Base tag=#input className={input(~error)} ?placeholder ?onChange ?type_ ?value />
    {switch error {
    | None => React.null
    | Some(error) =>
      <Typography
        fontWeight=[xs(#500)] m=[xs(0)] fontSize=[xs(1.2->#rem)] color=[xs(#hex("#ff5a5a"))]>
        {error->React.string}
      </Typography>
    }}
  </Box>
}
