let str = React.string

let map = (items, fn) =>
  Belt.Array.mapWithIndex(items, (i, value) => fn(i->Belt.Int.toString, value)) |> React.array
