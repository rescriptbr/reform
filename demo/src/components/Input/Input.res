open Render
module Option = Belt.Option
module Styles = Input_Styles

@react.component
let make = (~label, ~placeholder=?, ~type_=?, ~onChange, ~field, ~error) => {
  let className = Styles.wrapper(~hasError=Option.isSome(error))

  <div className>
    <label> {label->str} </label>
    <input ?placeholder ?type_ onChange={FormHelpers.updateField(field, onChange)} />
    {switch error {
    | Some(message) => <span> {message->str} </span>
    | None => React.null
    }}
  </div>
}
