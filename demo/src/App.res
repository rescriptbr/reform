open Ancestor

Emotion.injectGlobal({
  "html": {
    "fontSize": "10px",
  },
  "*": {
    "boxSizing": "border-box",
    "fontFamily": "Cairo",
  },
  "html, body, #root": {
    "background": "#fafafa",
    "padding": 0,
    "margin": 0,
    "width": "100%",
    "height": "100%",
  },
})

@react.component
let make = () => {
  let router = RescriptReactRouter.useUrl()
  <Box height=[xs(100.0->#pct)] display=[xs(#flex)] justifyContent=[xs(#center)]>
    <Box
      maxW=[xs(104.0->#rem)]
      width=[xs(100.0->#pct)]
      flexDirection=[xs(#column)]
      display=[xs(#flex)]>
      <Header />
      <Box
        width=[xs(100.0->#pct)]
        flexGrow=[xs(3.0->#num)]
        display=[xs(#flex)]
        justifyContent=[xs(#center)]
        alignItems=[xs(#center)]>
        {switch router.path {
        | list{}
        | list{"signup"} =>
          <SignUp />
        | _ => React.null
        }}
      </Box>
      <Box display=[xs(#flex)] justifyContent=[xs(#center)] py=[xs(3)]>
        <Typography tag=#p fontSize=[xs(1.8->#rem)] fontWeight=[xs(#600)]>
          {`Made with `->React.string}
          <Typography href="https://rescript-lang.org" target="_blank" tag=#a>
            {`ReScript`->React.string}
          </Typography>
          {` and `->React.string}
          <Typography href="https://reform.rescriptbrasil.org" target="_blank" tag=#a>
            {`ReForm`->React.string}
          </Typography>
        </Typography>
      </Box>
    </Box>
  </Box>
}
