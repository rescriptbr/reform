---
id: basicUsage
title: Basic usage
---

A basic usage of ReForm looks like this:

First, create a lenses module that will be passed as a param to the ReForm functor:

```ocaml
module StateLenses = [%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
```

Now you can provide the lenses module to the ReForm functor as argument:
```ocaml
module PostAddForm = BsReform.ReForm.Make(StateLenses);
```

After creating instantiating your form module, you can use it as a React hook:

```ocaml
[@react.component]
let make = () => {
  let reform =
    PostAddForm.use(
      ~validationStrategy=OnDemand,
      ~schema={
        PostAddForm.Validation.Schema([|
          StringMin(Title, 20),
          StringNonEmpty(Description),
          Custom(
            AcceptTerms,
            values =>
              values.acceptTerms == false
                ? Error("You must accept all the terms") : Valid,
          )
        |]);
      },
      ~onSubmit=
        ({state}) => {
          Js.log2("title", state.values.description);
          Js.log2("description", state.values.description);
          Js.log2("acceptTerms", state.values.description);
          None;
        },
      ~initialState={title: "", description: "", acceptTerms: false},
      (),
    );

  <PostAddForm.Provider value=reform>
    <form
      onSubmit={event => {
        ReactEvent.Synthetic.preventDefault(event);
        reform.submit();
      }}>
      <FieldString field=StateLenses.Title label="Title" />
      <FieldString field=StateLenses.Description label="Description" />
      <PostAddForm.Field
        field=StateLenses.AcceptTerms
        render={({handleChange, error, value}) =>
          <label>
            <p>
              <span> {"Accept terms? " |> React.string} </span>
              <input
                type_="checkbox"
                value={string_of_bool(value)}
                onChange={event =>
                  ReactEvent.Form.target(event)##checked |> handleChange
                }
              />
            </p>
            <p> {error->Belt.Option.getWithDefault("")->React.string} </p>
          </label>
        }
      />
      {reform.state.formState == Submitting
         ? <p> {React.string("Saving...")} </p>
         : <button type_="submit"> {"Submit" |> React.string} </button>}
    </form>
  </PostAddForm.Provider>;
};
```

## What?

Let's try to focus on the parts

