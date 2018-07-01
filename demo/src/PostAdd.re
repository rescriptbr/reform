module PostAddFormParams = {
  type state = {
    description: BsReform.Value.t,
    title: BsReform.Value.t,
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

module PostAddForm = BsReform.ReForm.Create(PostAddFormParams);

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
                 schema=[
                   (
                     `title,
                     Custom(
                       (
                         values =>
                           switch (values.title) {
                           | String(title) =>
                             Js.String.length(title)
                             |> (value => value > 20 || value < 5) ?
                               Some("Invalid title") : None
                           | _ => Some("Invalid")
                           }
                       ),
                     ),
                   ),
                   (`description, Required),
                 ]
                 onSubmit=(
                   ({values}) =>
                     PostAddMutation.make(
                       ~title=BsReform.Value.unwrapString(values.title, ""),
                       ~description=
                         BsReform.Value.unwrapString(values.description, ""),
                       (),
                     )
                     |> mutate
                 )
                 initialState={title: String(""), description: String("")}>
                 ...(
                      ({form, handleChange, handleSubmit, getFieldState}) =>
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
                              value=(
                                BsReform.Value.unwrapString(
                                  form.values.title,
                                  "",
                                )
                              )
                              onChange=(
                                BsReform.Helpers.handleStringChange(
                                  handleChange(`title),
                                )
                              )
                            />
                            <p>
                              (
                                getFieldState(`title)
                                |> (
                                  fun
                                  | Error(error) => Some(error)
                                  | _ => None
                                )
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.stringToElement
                              )
                            </p>
                          </label>
                          <label>
                            <span>
                              ("Description:" |> ReasonReact.stringToElement)
                            </span>
                            <textarea
                              value=(
                                BsReform.Value.unwrapString(
                                  form.values.description,
                                  "",
                                )
                              )
                              rows=4
                              onChange=(
                                BsReform.ReForm.Helpers.handleStringChange(
                                  handleChange(`description),
                                )
                              )
                            />
                            <p>
                              (
                                getFieldState(`description)
                                |> (
                                  fun
                                  | Error(error) => Some(error)
                                  | _ => None
                                )
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
