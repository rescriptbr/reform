open Ancestor

@react.component
let make = () => {
  <Box
    tag=#header
    px=[xs(3), md(6)]
    py=[xs(3), md(4)]
    display=[xs(#flex)]
    alignItems=[xs(#center)]
    justifyContent=[xs(#"space-between")]>
    <Typography
      fontSize=[xs(2.4->#rem), md(2.8->#rem)] fontWeight=[xs(#700)] letterSpacing=[xs(-0.04->#em)]>
      {`📋  ReForm`->React.string}
    </Typography>
    <Box tag=#ul listStyleType=[xs(#none)] display=[xs(#flex)] gap=[xs(1.2->#rem->#one)]>
      <Box tag=#li>
        <Typography
          fontWeight=[xs(#500)]
          fontSize=[xs(1.6->#rem)]
          color=[xs(#hex("#000"))]
          textDecorationStyle=[xs(#solid)]
          tag=#a
          href="/signup">
          {`Sign Up`->React.string}
        </Typography>
      </Box>
      <Box tag=#li>
        <Typography
          fontWeight=[xs(#500)]
          fontSize=[xs(1.6->#rem)]
          color=[xs(#hex("#000"))]
          textDecorationStyle=[xs(#solid)]
          tag=#a
          href="/login">
          {`Login`->React.string}
        </Typography>
      </Box>
    </Box>
  </Box>
}
