open Ancestor.Default

module Styles = {
  let floating = Emotion.css({
    "background": `#efefef`,
    "position": "absolute",
    "border": `solid 1px #efefef`,
    "top": 0,
    "right": 0,
    "borderBottomLeftRadius": 4,
    "padding": 6,
    "> span": {
      "fontSize": 12,
      "textTransform": `uppercase`,
      "fontWeight": `bold`,
      "color": `#7b7b7b`,
    },
  })
}

let default = props => {
  <Box
    overflow=[xs(#hidden)]
    position=[xs(#relative)]
    border=[(1->#px, #solid, #hex("#cecece"))->xs]
    p=[xs(4)]
    bgColor=[xs(#hex("#fff"))]
    borderRadius=[xs(1)]>
    <Box className=Styles.floating>
      <Typography tag=#span> {`Live preview`->React.string} </Typography>
    </Box>
    {props["children"]}
  </Box>
}
