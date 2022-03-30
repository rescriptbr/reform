open Ancestor.Default

let default = props => {
  <Box
    border=[(1->#px, #solid, #hex("#cecece"))->xs]
    p=[xs(4)]
    bgColor=[xs(#hex("#fff"))]
    borderRadius=[xs(1)]>
    {props["children"]}
  </Box>
}
