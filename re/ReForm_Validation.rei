module I18n: {
  type dictionary = {
    required: string,
    email: string
  };
  let ptBR: dictionary;
  let en: dictionary;
};

type error = option(string);

type errors = option(list(string));

type validation('values) =
  | Required
  | Email
  | Custom('values => error);

let getValidationError:
  (validation('b), ~values: 'b, ~value: string, ~i18n: I18n.dictionary) =>
  error;

let getValidationErrors:
  (
    list(validation('b)),
    ~values: 'b,
    ~value: string,
    ~i18n: I18n.dictionary
  ) =>
  errors;