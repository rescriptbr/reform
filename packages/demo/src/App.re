[@react.component]
let make = (~message) => {
  let url = ReasonReactRouter.useUrl();

  <div className="App">
    <div className="App-header">
      <img
        src="https://avatars2.githubusercontent.com/u/14410490?s=200&v=4"
        className="App-logo"
        alt="logo"
      />
      <h2> {React.string(message)} </h2>
    </div>
    <h1> {ReasonReact.string("ReFormNext demo")} </h1>
    <a href="/new-post"> "New post form demo "->React.string </a>
    <br />
    <a href="/fav-colors"> "Form with array field demo"->React.string </a>
    <br />
    <a href="/todo"> "Form with array todo demo"->React.string </a>
    <div style={ReactDOMRe.Style.make(~padding="100px", ())}>
      {switch (url.path) {
       | ["new-post"] => <PostAddNext />
       | ["fav-colors"] => <FavoriteColorsForm />
       | ["todo"] => <TodoListForm />
       | _ =>
         <p className="App-intro">
           {ReasonReact.string("Say hello to ReForm")}
         </p>
       }}
    </div>
  </div>;
};
