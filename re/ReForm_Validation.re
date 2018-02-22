/* Here the possible accepted validadors are defined */
/* Also their respective validation logic */
module I18n = {
  type dictionary = {
    required: string,
    email: string
  };
  let ptBR = {
    required: {j|Campo obrigatório|j},
    email: {j|Email inválido|j}
  };
  let en = {required: "Field is required", email: "Invalid email"};
};

type validation('values) =
  | Required
  | Email
  | Custom('values => option(string));

let getValidationError =
    ((_, validator), ~values, ~value, ~i18n: I18n.dictionary) =>
  switch validator {
  | Required => String.length(value) < 1 ? Some(i18n.required) : None
  | Custom(fn) => fn(values)
  | Email =>
    Js.Re.test(value, [%bs.re {|/\S+@\S+\.\S+/|}]) ? None : Some(i18n.email)
  };
