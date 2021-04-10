module Fields = %lenses(
  type state = {
    name: string,
    email: string,
  }
)

include ReForm.Make(Fields)

let initialState: Fields.state = {
  name: "",
  email: "",
}
