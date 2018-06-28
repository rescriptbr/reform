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

type validation('values) =
  | Required
  | Email
  | Custom('values => option(string))
  | CustomAsync('values => Js.Promise.t(option(string)));

let getValidationError =
    ((_, validator), ~values, ~value, ~i18n: I18n.dictionary) =>
  Js.Promise.(
    switch (validator) {
    | Required =>
      switch (value) {
      | Value.String(value) =>
        resolve(String.length(value) < 1 ? Some(i18n.required) : None)
      | _ => resolve(Some(i18n.required))
      }
    | Custom(fn) => resolve(fn(values))
    | CustomAsync(fn) => fn(values)
    | Email =>
      switch (value) {
      | Value.String(value) =>
        resolve(
          Js.Re.test(value, [%bs.re {|/\S+@\S+\.\S+/|}]) ?
            None : Some(i18n.email),
        )
      | _ => resolve(Some(i18n.required))
      }
    }
  );
