`*.html` -- HyperText Markup Language.

`*.md` -- Markdown

Markup? Markdown? Get it?

Ok, let's try another one.

`*.yaml` -- Yet Another Markup Language

`*.yml` -- YAML ~~Ain't~~ Markup Language

Hmm. A nonsensical, recursive acronym? Let me decode this...

```
cat << 'EOF' | rustc -C opt-level=3 -C strip=symbols -C lto=fat -C panic=abort -
> macro_rules! yaml {
>     () => {
>         format!("{} Ain't Markup Language", yaml!())
>     }
> }
> 
> fn main() {
>     println!("{}", yaml!());
> }
> ^D
-bash: warning: here-document at line 2 delimited by end-of-file (wanted 'EOF')
```
*deepbreath*
```text
error: recursion limit reached while expanding `yaml!`
 --> src/main.rs:3:45
  |
3 |         format!("{} Ain't Markup Language", yaml!())
  |                                             ^^^^^^^
...
8 |     println!("{}", yaml!());
  |                    ------- in this macro invocation
  |

error: aborting due to 1 previous error
```
CRAB!

PS:
This regex matches many OpenGL functions
gl(Bind|Gen|Disable|Enable)?(Vertex|Index|Uniform|Shader)(Arrays?|Element|Attrib)?(Pointer|Buffers?|Vertex|Fragment)?
