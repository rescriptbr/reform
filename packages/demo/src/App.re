[@react.component]
let make = (~message) => {
  let url = ReasonReactRouter.useUrl();

  <div className="p-5">
    <div className="flex align-center justify-center">
      <h2 className="text-2xl"> {React.string(message)} </h2>
    </div>
    <a className="text-bold" href="/new-post">
      "New post form demo "->React.string
    </a>
    <br />
    <a className="text-bold" href="/fav-colors">
      "Form with array field demo"->React.string
    </a>
    <br />
    <a className="text-bold" href="/todo">
      "Form with array todo demo"->React.string
    </a>
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
