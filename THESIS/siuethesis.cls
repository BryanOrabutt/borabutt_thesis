%%
%% This is file `siuethesis.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% siuethesis.dtx  (with options: `class')
%% 
%% This is a generated file.
%% 
%% Copyright (C) 2004 by Scott Pakin <scott+dtx@pakin.org>
%% 
%% This file may be distributed and/or modified under the conditions of
%% the LaTeX Project Public License, either version 1.2 of this license
%% or (at your option) any later version.  The latest version of this
%% license is in:
%% 
%%    http://www.latex-project.org/lppl.txt
%% 
%% and version 1.2 or later is part of all distributions of LaTeX version
%% 1999/12/01 or later.
%% 
%% siuethesis:  Document class for SIUE Thesis (not officially endorsed by SIUE)
%% Adam G. Weyhaupt (aweyhau@siue.edu)
%% Based on the thesis class by
%% N. Danner (ndanner@indiana.edu)
%% Copyright 1999
%% (N. Danner placed the copyright restriction; I have his permission to modify, but it is not clear to me what the license status of this file is.)

\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\ProvidesClass{siuethesis}
    [2015/06/09 v0.2.5 siuethesis class]
\newif\ifIUT@debugging \IUT@debuggingtrue \IUT@debuggingfalse
\newif\ifIUT@twoadvisors \IUT@twoadvisorstrue \IUT@twoadvisorsfalse
\def\IUT@dbgmsg#1{\ifIUT@debugging\message{#1}\fi}
\def\IUT@optionmsg#1{\message{  siuethesis option: #1}}
\def\dm@internal{}
\DeclareOption{draft}{
  \IUT@optionmsg{draft}
  \def\dm@internal{DRAFT: \today}
  \PassOptionsToClass{draft}{book}
}
\DeclareOption{final}{
  \IUT@optionmsg{final}
  \def\dm@internal{}
  \PassOptionsToClass{final}{book}
}

\DeclareOption{ms}{
  \AtEndOfClass{\degree{Master of Science}}
}
\DeclareOption{ma}{
  \AtEndOfClass{\degree{Master of Arts}}
}

\DeclareOption{single}{
  \AtEndOfClass{\def\IUT@blstretch{1}}
}

\DeclareOption{twoadvisors}{
  \IUT@twoadvisorstrue
}

\DeclareOption{debug}{
  \IUT@debuggingtrue
}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}
\ExecuteOptions{abstractsigs}
\ProcessOptions

\PassOptionsToClass{oneside}{book}
\LoadClass{book}[1995/01/27]
\RequirePackage{amsmath,subfig,amssymb,amsthm,fancyhdr,xspace}
\RequirePackage[left=1.5in,top=1in,right=0.75in,bottom=0.8in,includehead, heightrounded,letterpaper,head=14.5pt]{geometry} %sets margins
\usepackage[titles,subfigure]{tocloft}
\setcounter{tocdepth}{2}
\usepackage[normalem]{ulem}
\newcounter{hassections}[chapter]

\renewcommand{\author}[1]{\gdef\@author{#1}}\let\@author\relax
\renewcommand{\title}[1]{\gdef\@title{#1}}\let\@title\relax

\def\submitdate#1{\gdef\@submitdate{#1}}\let\@submitdate\relax
\def\defensedate#1{\gdef\@defensedate{#1}}\let\@defensedate\relax
\def\department#1{\gdef\@department{#1}}\department{Mathematics and Statistics}
\def\major#1{\gdef\@major{#1}}\major{Mathematics}
\def\departmentname#1{\gdef\@departmentname{#1}}\departmentname{Department}
\def\degree#1{\gdef\@degree{#1}}\degree{Master of Science}
\def\undergraddegree#1{\gdef\@highestdegree{#1}}
\def\highestdegree#1{\gdef\@highestdegree{#1}}\highestdegree{Bachelor of Science}
\def\advisor#1{%
\@ifnextchar\bgroup%
   {\@twoargadvisor{#1}}
   {\@oneargadvisor{#1}}
}
\def\@oneargadvisor#1{%
   \gdef\@principaladvisor{#1}\gdef\@advisorhonoriffic{}%
}
\def\@twoargadvisor#1#2{%
   \gdef\@principaladvisor{#2}\gdef\@advisorhonoriffic{#1\@\xspace}%
}\let\@principaladvisor\relax
%%%\def\secondadvisor#1{\gdef\@secondaryadvisor{#1}}\let\@secondaryadvisor\relax
\def\secondadvisor#1{%
\@ifnextchar\bgroup%
   {\@twoargsecadvisor{#1}}
   {\@oneargsecadvisor{#1}}
}
\def\@oneargsecadvisor#1{%
   \gdef\@secondaryadvisor{#1}\gdef\@secondaryadvisorhonoriffic{}%
}
\def\@twoargsecadvisor#1#2{%
   \gdef\@secondaryadvisor{#2}\gdef\@secondaryadvisorhonoriffic{#1\@\xspace}%
}\let\@secondaryadvisor\relax
%%%\def\secondreader#1{\gdef\@secondreader{#1}}\let\@secondreader\relax
\def\secondreader#1{%
\@ifnextchar\bgroup%
   {\@twoargsecreader{#1}}
   {\@oneargsecreader{#1}}
}
\def\@oneargsecreader#1{%
   \gdef\@secondreader{#1}\gdef\@secondreaderhonoriffic{}%
}
\def\@twoargsecreader#1#2{%
   \gdef\@secondreader{#2}\gdef\@secondreaderhonoriffic{#1\@\xspace}%
}\let\@secondreader\relax\let\@secondreaderhonoriffic\relax%
%%%\def\thirdreader#1{\gdef\@thirdreader{#1}}\let\@thirdreader\relax
\def\thirdreader#1{%
\@ifnextchar\bgroup%
   {\@twoargthirdreader{#1}}
   {\@oneargthirdreader{#1}}
}
\def\@oneargthirdreader#1{%
   \gdef\@thirdreader{#1}\gdef\@thirdreaderhonoriffic{}%
}
\def\@twoargthirdreader#1#2{%
   \gdef\@thirdreader{#2}\gdef\@thirdreaderhonoriffic{#1\@\xspace}%
}\let\@thirdreader\relax\let\@thirdreaderhonoriffic\relax%
%%%\def\fourthreader#1{\gdef\@fourthreader{#1}}\let\@fourthreader\relax
\def\fourthreader#1{%
\@ifnextchar\bgroup%
   {\@twoargfourthreader{#1}}
   {\@oneargfourthreader{#1}}
}
\def\@oneargfourthreader#1{%
   \gdef\@fourthreader{#1}\gdef\@fourthreaderhonoriffic{}%
}
\def\@twoargfourthreader#1#2{%
   \gdef\@fourthreader{#2}\gdef\@fourthreaderhonoriffic{#1\@\xspace}%
}\let\@fourthreader\relax\let\@fourthreaderhonoriffic\relax%
%%%\def\fifthreader#1{\gdef\@fifthreader{#1}}\let\@fifthreader\relax
\def\fifthreader#1{%
\@ifnextchar\bgroup%
   {\@twoargfifthreader{#1}}
   {\@oneargfifthreader{#1}}
}
\def\@oneargfifthreader#1{%
   \gdef\@fifthreader{#1}\gdef\@fifthreaderhonoriffic{}%
}
\def\@twoargfifthreader#1#2{%
   \gdef\@fifthreader{#2}\gdef\@fifthreaderhonoriffic{#1\@\xspace}%
}\let\@fifthreader\relax\let\@fifthreaderhonoriffic\relax%
\def\abstracttext#1{\gdef\@abstracttext{#1}}
\ifIUT@twoadvisors\abstracttext{Chairpersons: Professors \check@val\@principaladvisor ~and \check@val\@secondaryadvisor}\else\abstracttext{Chairperson: Professor \check@val\@principaladvisor}\fi%
\def\refname#1{\gdef\@refname{#1}}\refname{REFERENCES}

\def\copyrightyear#1{\gdef\@copyrightyear{#1}}
\let\@copyrightyear\relax


\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                       \if@mainmatter
                         \refstepcounter{chapter}%
                         \typeout{\@chapapp\space\thechapter.}%
                         \addcontentsline{toc}{chapter}%
                                   {\protect\numberline{\thechapter .}#1}%
                       \else
                         \addcontentsline{toc}{chapter}{#1}%
                       \fi
                    \else
                      \addcontentsline{toc}{chapter}{#1}%
                    \fi
                    \chaptermark{#1}%
                    \addtocontents{lof}{\protect\addvspace{10\p@}}%
                    \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi}


\newenvironment{abstract}{\clearpage
\label{abstract}
  \addcontentsline{toc}{part}{ABSTRACT}
\begin{center}{ABSTRACT}\\ \end{center}
\begin{center}\MakeUppercase{\check@val\@title}\\ \end{center}
\begin{center}by\\ \end{center}
\begin{center}\MakeUppercase{\check@val\@author}\\ \end{center}
 \begin{center}\@abstracttext%
\hspace{0pt}\\ \end{center}  \renewcommand{\baselinestretch}{2.0}\selectfont
}{\clearpage    \renewcommand{\baselinestretch}{1.0}
}{\clearpage
}

  \def\maketitle{
    \pagenumbering{alph}
\begin{titlepage}
\begin{sloppypar}
\begin{center}
\ \\[1\baselineskip]
\check@val\@title\\[1\baselineskip]
by \check@val\@author, \check@val\@highestdegree \\[6\baselineskip]
A Thesis Submitted in Partial \\
Fulfillment of the Requirements \\
for the Degree of \\
\check@val\@degree\ \\
in the field of \check@val\@major\ \\[5\baselineskip]
Advisory Committee:\\[1\baselineskip]
\ifIUT@twoadvisors \else \@advisorhonoriffic\check@val\@principaladvisor, Chair \\[1\baselineskip]\fi
\ifIUT@twoadvisors \@advisorhonoriffic\check@val\@principaladvisor, Co-chair \\[1\baselineskip] \fi
\ifIUT@twoadvisors \@secondaryadvisorhonoriffic\check@val\@secondaryadvisor, Co-chair \\[1\baselineskip] \fi
\@secondreaderhonoriffic\check@val\@secondreader\ \\[1\baselineskip]
\@thirdreaderhonoriffic\@thirdreader\ \\[1\baselineskip]
\@fourthreaderhonoriffic\@fourthreader\ \\[1\baselineskip]
\@fifthreaderhonoriffic\@fifthreader\ \\[1\baselineskip]
\ \\[4\baselineskip]
Graduate School \\
Southern Illinois University Edwardsville \\
\check@val\@submitdate
\end{center}
\end{sloppypar}

\end{titlepage}
  }

\newenvironment{acknowledgements}{\clearpage
\label{acknowledgements}
  \addcontentsline{toc}{part}{ACKNOWLEDGEMENTS}
\begin{center}{ACKNOWLEDGEMENTS} \end{center}
\renewcommand{\baselinestretch}{2.0}\selectfont
}{\clearpage \renewcommand{\baselinestretch}{1.0}\selectfont
}

  \def\copyrightpage{%
    \typeout{Copyright Page}%
    \newcounter{temppage}  % place to store the current page number
\setcounter{temppage}{\arabic{page}}
      \pagenumbering{Alph}
    \thispagestyle{empty} %no page number
\addtocounter{page}{-1} %ignore this page when counting
    \hbox{}\vfill%
    \begin{center}%
    \copyright ~Copyright by \check@val\@author ~\check@val\@submitdate \\%
    All rights reserved%
    \end{center}%
    \vfill%
    \newpage%
           \pagenumbering{roman}%
       \setcounter{page}{\thetemppage}%
  }

  \newcommand{\makededication}[1][1in]{
    \ifvoid\dedicationbox\else
    \typeout{Dedication}
    \hbox{}\vskip#1\unvbox\dedicationbox\vfill%
    \newpage
    \fi
  }

\let\tocbaselineskip\baselineskip
%%%\renewcommand*{\l@part}{\@dottedtocline{-1}{0em}{2.3em}}
%%%\renewcommand*{\l@chapter}{\@dottedtocline{0}{1.8em}{2em}} %1.8 + 1.5 = 3.3
  \def\l@part#1#2{{
    \@dottedtocline{-1}{0em}{2.3em}{#1}{#2} \vskip \baselineskip}}
  \def\l@chapter#1#2{{
 \vskip \tocbaselineskip
    \hangindent=1em\@dottedtocline{0}{1.8em}{2em}{#1}{#2} }}
\renewcommand*{\l@section}{\hangindent=1em\@dottedtocline{1}{3.8em}{2em}}
\renewcommand*{\l@subsection}{\hangindent=1em\@dottedtocline{2}{8em}{3em}}
\renewcommand*{\l@subsubsection}{\hangindent=1em\@dottedtocline{3}{1.8em}{2.3em}}
\renewcommand*\l@figure{\setlength\@tempdima{2.9em}%
\hangindent=1.8em\@dottedtocline{1}{1em}{\@tempdima}}
    \renewcommand*\l@table{\setlength\@tempdima{2.9em}%
    \hangindent=1.8em\@dottedtocline{1}{1em}{\@tempdima}}
%% have a look at http://www.tex.ac.uk/cgi-bin/texfaq2html?label=findwidth

\renewcommand{\bibname}{\check@val\@refname}

\def\tableofcontents{
  \bgroup
  \renewcommand{\baselinestretch}{1.0}\renewcommand{\@tocrmarg}{5em plus1fil} %singlespacing and no hyphenation
\begin{center}{TABLE OF CONTENTS}\\ \end{center}
%%\begin{center}\hspace{0pt}\hspace{0pt}\\ \end{center} % S. Robinson prefers this spacing
  \@starttoc{toc}%%\contentsname
  \egroup
}
\def\listoffigures{
  \bgroup
  \renewcommand{\baselinestretch}{1.0}\renewcommand{\@tocrmarg}{5em plus1fil}
  \addcontentsline{toc}{part}{LIST OF FIGURES}
\begin{center}{LIST OF FIGURES}\\ \end{center}
  \@starttoc{lof}%%\listfigurename
  \egroup
}
\def\listoftables{
 \bgroup
  \renewcommand{\baselinestretch}{1.0}\renewcommand{\@tocrmarg}{5em plus1fil}
  \addcontentsline{toc}{part}{LIST OF TABLES}
\begin{center}{LIST OF TABLES}\\ \end{center}
  \@starttoc{lot}%%\listfigurename
  \egroup
}

\def\frontmatter{\cleardoublepage\pagenumbering{roman}\setcounter{page}{2}\fancypagestyle{plain}{%
\fancyhf{} %% clear all header and footer fields
  \renewcommand{\headrulewidth}{0pt}
\fancyfoot[C]{\thepage} %% except the center
\fancyhead[L]{} %% except the center
}
\pagestyle{plain}}

\def\mainmatter{
  \cleardoublepage
  \pagenumbering{arabic}
  \addtocontents{toc}{\noindent\protect\mbox{Chapter}\par}
\addtocontents{lof}{\noindent\protect\mbox{Figure}\hfill\protect\makebox[1em]{Page}\par}
\addtocontents{lot}{\noindent\protect\mbox{Table}\hfill\protect\makebox[1em]{Page}\par}
  \pagestyle{fancy}
  \renewcommand{\headrulewidth}{0pt}
\fancypagestyle{plain}{%
\fancyhf{} %% clear all header and footer fields
\fancyhead[R]{\thepage} %% except the center
\fancyhead[L]{\normalfont\scriptsize \tt \dm@internal}
}
\pagestyle{fancy}
\lhead{\normalfont\scriptsize \tt \dm@internal}
\chead{}
\rhead{\thepage}
\lfoot{ }
\cfoot{}
\rfoot{}
  \renewcommand{\baselinestretch}{\IUT@blstretch}
  \normalfont
}

\def\references{
    \newcommand{\BibHangIndent}{% %from https://groups.google.com/forum/#!topic/comp.text.tex/jZirZPWWoiA
        \setlength{\labelwidth}{0pt}%
        \setlength{\leftmargin}{2\leftmargini}%
        \setlength{\itemindent}{-\leftmargin}%
        \renewcommand{\makelabel}[1]{\hspace\labelsep####1}%
    }
  \cleardoublepage
  \pagestyle{fancy}
  \renewcommand{\headrulewidth}{0pt}
\fancypagestyle{plain}{%
\fancyhf{} %% clear all header and footer fields
\fancyhead[R]{\thepage} %% except the center
\fancyhead[L]{\normalfont\scriptsize \tt \dm@internal}
}
\pagestyle{fancy}
\lhead{\normalfont\scriptsize \tt \dm@internal}
\chead{}
\rhead{\thepage}
\lfoot{}
\cfoot{}
\rfoot{}
  \addtocontents{toc}{\noindent\protect\vskip\baselineskip\par}
\addcontentsline{toc}{part}{\check@val\@refname}
    \renewcommand{\baselinestretch}{1.0}
      \normalfont
      \begin{center}{REFERENCES}\\ \end{center}
}

\newdimen\linespacing
  \normalsize \linespacing=\baselineskip
  \newdimen\normalparindent
\normalparindent=18pt
\parindent=\normalparindent
\def\section{\ifnum \value{hassections} < 1 \addtocontents{toc}{\noindent\protect \vskip \baselineskip \par}\fi \stepcounter{hassections} \@startsection{section}{1}%
  \z@{.7\linespacing\@plus\linespacing}{.5\linespacing}%
  %    \end{macrocode}
  %\begin{macrocode}
  {\normalfont\flushleft\uline}}\def\subsection{\@startsection{subsection}{2}%
  \z@{.7\linespacing\@plus\linespacing}{.5\linespacing}%
  {\normalfont\itshape\flushleft}}
\def\subsubsection{\@startsection{subsubsection}{3}%
  \z@\z@{-.5em}%
  {\normalfont\itshape\flushleft}}
\def\paragraph{\@startsection{paragraph}{4}%
  \normalparindent\z@{-\fontdimen2\font}%
  \normalfont}
\def\subparagraph{\@startsection{subparagraph}{5}%
  \z@\z@{-\fontdimen2\font}%
  \normalfont}
  %    \end{macrocode}

\def\@makechapterhead#1{%
  {\parindent \z@ \raggedright \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
        \normalsize \centering\MakeUppercase\@chapapp\space \thechapter
        \par\nobreak
        \vskip 10\p@
      \fi
    \fi
    \interlinepenalty\@M
     \centering#1\par\nobreak
    \vskip 20\p@
  }}

  \def\@makeschapterhead#1{%
  {\parindent \z@ \raggedright \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
        \normalsize \centering\MakeUppercase\@chapapp\space \thechapter
        \par\nobreak
        \vskip 10\p@
      \fi
    \fi
    \interlinepenalty\@M
     \centering#1\par\nobreak
    \vskip 20\p@
  }}

\renewenvironment{thebibliography}[1]
     {%\begin{center}{\check@val\@refname}\\ \end{center}
      \@mkboth{\MakeUppercase\bibname}{\MakeUppercase\bibname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin20pt% change 20 pt according to your needs
            \advance\leftmargin\labelsep
            \setlength\itemindent{-20pt}% change using the inverse of the length used before
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}

\def\backmatter{
  \newpage
  \pagestyle{empty}
  \renewcommand{\baselinestretch}{1}
  \normalfont
}

\def\singleappendix{
    \renewcommand{\baselinestretch}{2.0}\selectfont
  \cleardoublepage
    \addcontentsline{toc}{part}{APPENDIX}
\addtocontents{toc}{\def\tocbaselineskip{0em}} %cause single spacing of appendices in TOC
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \gdef\@chapapp{\appendixname}%
  \gdef\thechapter{\@Alph\c@chapter}
}

\def\multipleappendices{
    \renewcommand{\baselinestretch}{2.0}\selectfont
  \cleardoublepage
    \addcontentsline{toc}{part}{APPENDICES}
     \addtocontents{toc}{\vskip-\baselineskip} %correct for an extra space caused by double spacing chapters
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \gdef\@chapapp{\appendixname}%
  \gdef\thechapter{\@Alph\c@chapter}
}

\def\check@val#1{%
  \ifx#1\relax%
    \typeout{}%
    \typeout{!!!!!!!!}%
    \typeout{Warning: #1 not set!}%
    \typeout{!!!!!!!!}%
    \hbox{???}%
  \else%
    #1%
  \fi%
}
\def\IUT@blstretch{1.67}

%%\pagestyle{chapter}
\pagenumbering{arabic}
\normalsize

\endinput
%%
%% End of file `siuethesis.cls'.
