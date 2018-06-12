module type Config = {
  type fields;
  type state;
  type schema;
  let lens: list((fields, state => Value.t, (state, Value.t) => state));
};

module Make = (Config: Config) => {
  type state =
    | Validating
    | Valid
    | Error(string)
    | Dirty
    | Pristine;
  type value = Value.t;
  let getFieldLens = field =>
    /* TODO handle exception */
    Config.lens |> List.find(((fieldName, _, _)) => fieldName === field);
  let getNewFieldState = (field, values, value, schema, i18n) =>
    schema
    |> List.filter(((fieldName, _)) => fieldName === field)
    |> Belt.List.head
    |. Belt.Option.map(fieldSchema =>
         Validation.getValidationError(fieldSchema, ~values, ~value, ~i18n)
       )
    |. Belt.Option.map(
         Js.Promise.then_(error =>
           error
           |> (
             fun
             | Some(error) => Error(error)
             | None => Valid
           )
           |> Js.Promise.resolve
         ),
       );
  let handleChange = ((field, value), values) => {
    let (_, _, setter) = getFieldLens(field);
    setter(values, value);
  };
};
