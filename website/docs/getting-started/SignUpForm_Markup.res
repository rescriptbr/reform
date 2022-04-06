open Ancestor.Default

module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
    password: string,
  }
)

module Form = ReForm.Make(FormFields)

@react.component
let make = () => {
  let handleSubmit = ({state}: Form.onSubmitAPI) => {
    Js.log(state.values)

    None
  }

  let _form = Form.use(
    ~initialState={name: "", email: "", password: ""},
    ~onSubmit=handleSubmit,
    ~validationStrategy=OnDemand,
    ~schema={
      open Form.Validation

      Schema(string(~min=3, Name) + string(~min=8, Password) + email(~error="Invalid email", Email))
    },
    (),
  )

  <Box display=[xs(#flex)] flexDirection=[xs(#column)] alignItems=[xs(#center)]>
    <Box tag=#form maxW=[xs(320->#px)] width=[xs(100.0->#pct)]>
      <Typography tag=#h1 fontSize=[xs(24->#px)] fontWeight=[xs(#700)] mb=[xs(1)]>
        {"Sign up"->React.string}
      </Typography>
      <Box> <Input placeholder="Your name" /> </Box>
      <Box mt=[xs(1)]> <Input placeholder="Your email" /> </Box>
      <Box mt=[xs(1)]> <Input type_="password" placeholder="Password" /> </Box>
      <Box mt=[xs(1)]> <Button> {"Submit"->React.string} </Button> </Box>
    </Box>
  </Box>
}
