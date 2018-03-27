/* Here the possible accepted validadors are defined */
/* Also their respective validation logic */
module I18n = {
  type dictionary = {
    required: string,
    email: string,
  };
  let ptBR = {
    required: {j|Campo obrigatório|j},
    email: {j|Email inválido|j},
  };
  let en = {required: "Field is required", email: "Invalid email"};
};

type error = option(string);

type errors = option(list(string));

type validation('values) =
  | Required
  | Email
  | Custom('values => error);

/* turn list(error) => errors */
let squeeze = (errorList: list(error)) => {
  let errorList_ = errorList |> List.filter(error => error != None);
  List.length(errorList_) > 0 ?
    Some(
      errorList_
      |> List.map(opt =>
           switch (opt) {
           | Some(msg) => msg
           | None => ""
           }
         ),
    ) :
    None;
};

let getValidationError =
    (validator, ~values, ~value, ~i18n: I18n.dictionary)
    : error =>
  switch (validator) {
  | Required => String.length(value) > 0 ? None : Some(i18n.required)
  | Custom(fn) => fn(values)
  | Email =>
    String.length(value) == 0
    || Js.Re.test(value, [%bs.re {|/\S+@\S+\.\S+/|}]) ?
      None : Some(i18n.email)
  };

let getValidationErrors =
    (validators, ~values, ~value, ~i18n: I18n.dictionary)
    : errors => {
  let errors =
    validators
    |> List.map(validator =>
         getValidationError(validator, ~values, ~value, ~i18n)
       );
  squeeze(errors);
};
