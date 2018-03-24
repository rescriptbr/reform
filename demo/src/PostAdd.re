module PostAddFormParams = {
  type state = {
    description: string,
    title: string,
  };
  type fields = [ | `title | `description];
  /* (fieldName, getter, setter) */
  let lens = [
    (`title, s => s.title, (s, title) => {...s, title}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
  ];
};

module PostAddForm = ReForm.Create(PostAddFormParams);

module PostAddMutation = [%graphql
  {|
  mutation PostAddMutation ($description: String!, $title: String!) {
    createPost(description: $description, title: $title) {
      id
      title
      description
    }
  }
|}
];

module MutationContainer = Apollo.Client.Mutation;

let component = ReasonReact.statelessComponent("PostAdd");

let make = _children => {
  ...component,
  render: _self =>
    <MutationContainer>
      ...(
           (mutate, result) =>
             switch (result) {
             | Loading => <p> (ReasonReact.stringToElement("Loading...")) </p>
             | Failed(_error) =>
               <p>
                 (ReasonReact.stringToElement("Something went wrong..."))
               </p>
             | Loaded(result) =>
               let data = PostAddMutation.parse(result);
               <div>
                 <h2>
                   (
                     data##createPost
                     |> Belt.Option.map(_, post =>
                          "Post#" ++ post##id ++ " " ++ post##title
                        )
                     |> Belt.Option.getWithDefault(_, "")
                     |> ReasonReact.stringToElement
                   )
                 </h2>
                 <p>
                   (
                     data##createPost
                     |> Belt.Option.map(_, post => post##description)
                     |> Belt.Option.getWithDefault(_, "")
                     |> ReasonReact.stringToElement
                   )
                 </p>
               </div>;
             | NotCalled =>
               <PostAddForm
                 schema=[(`title, Required), (`description, Required)]
                 onSubmit=(
                   ({values}) =>
                     PostAddMutation.make(
                       ~title=values.title,
                       ~description=values.description,
                       (),
                     )
                     |> mutate
                 )
                 initialState={title: "", description: ""}>
                 ...(
                      ({form, handleChange, handleSubmit, getErrorForField}) =>
                        <form
                          onSubmit=(
                            event => {
                              ReactEventRe.Synthetic.preventDefault(event);
                              handleSubmit();
                            }
                          )>
                          <label>
                            <span>
                              ("Title:" |> ReasonReact.stringToElement)
                            </span>
                            <input
                              value=form.values.title
                              onChange=(
                                ReForm.Helpers.handleDomFormChange(
                                  handleChange(`title),
                                )
                              )
                            />
                            <p>
                              (
                                getErrorForField(`title)
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.stringToElement
                              )
                            </p>
                          </label>
                          <label>
                            <span>
                              ("Description:" |> ReasonReact.stringToElement)
                            </span>
                            <input
                              value=form.values.description
                              onChange=(
                                ReForm.Helpers.handleDomFormChange(
                                  handleChange(`description),
                                )
                              )
                            />
                            <p>
                              (
                                getErrorForField(`description)
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.stringToElement
                              )
                            </p>
                          </label>
                          <button _type="submit">
                            ("Submit" |> ReasonReact.stringToElement)
                          </button>
                        </form>
                    )
               </PostAddForm>
             }
         )
    </MutationContainer>,
};
