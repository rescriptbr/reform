---
id: basicUsage
title: Basic usage
---

A basic usage of ReForm looks like this:

First, create a lenses module that will be passed as a param to the ReFormNext functor:

```reason
module StateLenses = [%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
```

Now you can generate the actual form container component:
```reason
module PostAddForm = ReFormNext.Make(StateLenses);
```

After creating the container module, you can use it as a React component:

```reason
[@react.component]
let make = () => {
  let {state, submit, getFieldState, handleChange}: PostAddForm.api =
    PostAddForm.use(
      ~schema={
        PostAddForm.Validation.Schema([|
          Custom(
            Title,
            values =>
              Js.String.length(values.title) > 20
                ? Error("Keep it short!") : Valid,
          ),
          StringNonEmpty(Description),
          Custom(
            AcceptTerms,
            values =>
              values.acceptTerms == false
                ? Error("You must accept all the terms") : Valid,
          ),
        |]);
      },
      ~onSubmit=
        ({state}) => {
          yourLogic()

          None;
        },
      ~initialState={title: "", description: "", acceptTerms: false},
      (),
    );

    <form
      onSubmit={event => {
        ReactEvent.Synthetic.preventDefault(event);
        submit();
      }}>
      <label>
        <span> {"Title:" |> React.string} </span>
        <input
          value={state.values.title}
          onChange={ReForm.Helpers.handleDomFormChange(handleChange(Title))}
        />
        <p>
          {getFieldState(Field(Title))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> React.string}
        </p>
      </label>
      <label>
        <span> {"Description:" |> React.string} </span>
        <textarea
          value={state.values.description}
          rows=4
          onChange={ReForm.Helpers.handleDomFormChange(
            handleChange(Description),
          )}
        />
        <p>
          {getFieldState(Field(Description))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> React.string}
        </p>
      </label>
      <label>
        <p>
          <span> {"Accept terms? " |> React.string} </span>
          <input
            type_="checkbox"
            value={string_of_bool(state.values.acceptTerms)}
            onChange={event =>
              ReactEvent.Form.target(event)##checked
              |> handleChange(AcceptTerms)
            }
          />
        </p>
        <p>
          {getFieldState(Field(AcceptTerms))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> React.string}
        </p>
      </label>
      {state.formState == Submitting
         ? <p> {React.string("Saving...")} </p>
         : <button type_="submit"> {"Submit" |> React.string} </button>}
    </form>
};
```
