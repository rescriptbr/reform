open CssJs

let wrapper = (~hasError) =>
  style(.[
    display(#flex),
    flexDirection(#column),
    width(100.0->pct),
    position(relative),
    paddingBottom(2.4->rem),
    selector(
      "> label",
      [fontSize(1.6->rem), fontWeight(600->#num), color(white), marginBottom(0.4->rem)],
    ),
    selector(
      "> input",
      [
        hasError ? border(1->px, solid, Theme.Colors.red) : borderStyle(none),
        width(100.0->pct),
        fontSize(1.6->rem),
        borderRadius(4->px),
        backgroundColor("313131"->hex),
        color("fafafa"->hex),
        boxShadow(none),
        padding2(~h=1.6->rem, ~v=zero),
        height(52->px),
        outlineStyle(none),
      ],
    ),
    selector(
      "> span",
      [
        bottom(zero),
        left(zero),
        position(absolute),
        fontSize(1.2->rem),
        marginTop(1.2->rem),
        fontWeight(#bold),
        color(Theme.Colors.red),
      ],
    ),
  ])
