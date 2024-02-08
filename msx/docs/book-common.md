---
classoption:
- oneside
titlepage: true
titlepage-rule-color: "fcc603"
titlepage-rule-height: 4
fontsize: 9pt
papersize: A5
numbersections: true
secnumdepth: 2
colorlinks: true
listings-disable-line-numbers: true
geometry:
- top=2.3cm
- bottom=3cm
- left=1.8cm
- right=1.8cm
header-includes:
- |
  ```{=latex}
  \usepackage{awesomebox}
  \usepackage{wrapfig}
  \usepackage{background}
  \renewcommand{\linethickness}{10pt}
  \definecolor{abnote}{RGB}{255, 204, 0}
  \definecolor{yellow}{RGB}{255, 204, 0}
  \definecolor{red}{RGB}{208, 72, 72}
  \definecolor{orange}{RGB}{243, 185, 95}
  \definecolor{blue}{RGB}{104, 149, 210}
  \backgroundsetup{
    position=current page.east,
    angle=-90,
    nodeanchor=east,
    vshift=-3mm,
    opacity=0,
    scale=3,
    contents=Draft
  }
  ```
pandoc-latex-environment:
  noteblock: [note]
  tipblock: [tip]
  warningblock: [warning]
  cautionblock: [caution]
  importantblock: [important]
hyperrefoptions:
  draft
...
