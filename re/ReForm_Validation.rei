module I18n:
  {
    type dictionary = { required: string, email: string, };
    let ptBR: dictionary;
    let en: dictionary;
  };
type validation('values) =
    Required
  | Email
  | Custom(('values) => option(string));
let getValidationError:
  (('a, validation('b)), ~values: 'b, ~value: string,
  ~i18n: I18n.dictionary) => option(string);
