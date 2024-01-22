open Ancestor

module Lenses = %lenses(
  type state = {
    email: string,
    password: string,
  }
)

module Form = ReForm.Make(Lenses)

let schema = {
  open Form.Validation

  schema([email(Email), nonEmpty(Password)])
}

@react.component
let make = () => {
  let handleSubmit = ({state}: Form.onSubmitAPI) => {
    Js.Console.log2("Submitted => ", state.values)
    None
  }

  let form = Form.use(
    ~onSubmit=handleSubmit,
    ~validationStrategy=OnDemand,
    ~schema,
    ~initialState={email: "", password: ""},
    (),
  )
  <Form.Provider value=form>
    <Box
      px=[xs(4)]
      width=[xs(100.0->#pct)]
      height=[xs(100.0->#pct)]
      justifyContent=[xs(#center)]
      alignItems=[xs(#center)]
      display=[xs(#flex)]>
      <Box
        display=[xs(#flex)]
        flexDirection=[xs(#column)]
        gap=[xs(1.6->#rem->#one)]
        width=[xs(100.0->#pct)]
        maxW=[xs(32.4->#rem)]>
        <Typography tag=#h1 m=[xs(0)] fontSize=[xs(3.2->#rem)]>
          {`Login`->React.string}
        </Typography>
        <Form.Field
          field=Email
          render={field =>
            <Input
              value={field.value}
              onChange={ReForm.Helpers.handleChange(field.handleChange)}
              placeholder="Email"
              error=?field.error
            />}
        />
        <Form.Field
          field=Password
          render={field =>
            <Input
              value={field.value}
              onChange={ReForm.Helpers.handleChange(field.handleChange)}
              type_="password"
              placeholder="Password"
              error=?field.error
            />}
        />
        <Button label="Login" onClick={_ => form.submit()} />
        <Typography
          color=[xs(#hex("#000"))]
          href="https://github.com/rescriptbr/reform/blob/master/demo/src/pages/Login.res"
          target="_blank"
          textAlign=[xs(#center)]
          fontSize=[xs(1.6->#rem)]
          letterSpacing=[xs(-0.04->#em)]
          fontWeight=[xs(#400)]
          my=[xs(2)]
          tag=#a>
          {`See source on Github 🔎`->React.string}
        </Typography>
      </Box>
    </Box>
  </Form.Provider>
}
