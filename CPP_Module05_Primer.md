# C++ Module 05 — Repetition and Exceptions

### A mentor's reader for the Bureaucrat / Form project

> **How to use this document.** Read it once, start to finish, before you touch any code. It will not hand you the project — by design, and because the subject's own AI chapter is right: the *reasoning* is the point. What it *will* do is give you the mental models so that when you sit down, you're solving the real problem (modeling a bureaucracy with rules and failure) instead of fighting the language. Examples here are deliberately generic (`Account`, `Sensor`, `Animal`) — never Bureaucrat/Form logic — so they illustrate a mechanism without doing your thinking for you.
>
> Keep it open in a second pane while you work. When you hit a wall, the relevant section is your first stop *before* you reach for anything else.

---

## 0. The 30-second map

This module looks like it's about an absurd office simulator. It isn't. It's about **what your program does when something goes wrong**, and **how to write code that's open to extension without rewriting it**. Everything else is set dressing.

The four exercises are a staircase, each adding exactly one new idea:

| Ex | Surface task | The concept you're actually learning |
|----|-------------|--------------------------------------|
| 00 | `Bureaucrat` with a grade 1–150 | **Exceptions** — throwing/catching, custom exception classes, validating in constructors |
| 01 | `Form` that can be signed | **Objects interacting** through exceptions; one class's method throwing for another to catch |
| 02 | `AForm` + 3 concrete forms that *do* things | **Abstract classes & polymorphism** — a base type with behavior filled in by children |
| 03 | `Intern` that builds forms by name | **The factory pattern** — choosing a type at runtime without an if/else swamp |

If you internalize *exceptions* (00–01) and *abstract polymorphism* (02), exercise 03 is a victory lap. So that's where the depth of this document goes.

---

## 1. The one genuinely new beast: Exceptions

Everything in Modules 00–04 was about *structure* — how to model things with classes. Module 05's headline idea is about *control flow when reality refuses to cooperate*. This is worth slowing down for, because it's a different way of thinking than anything in C.

### 1.1 The problem exceptions solve

In C, when a function can fail, you have ugly options. Return a special value (`-1`, `NULL`) and hope the caller checks it. Set `errno`. Pass in a pointer for an error code. The fatal weakness of all of these: **the caller can ignore them**, and the error has to be manually relayed up through every single layer of function calls. A failure 6 levels deep has to be checked and re-returned 6 times to reach code that can handle it.

Consider a constructor. In C you don't really have them, but in C++ you do — and a constructor *can't return a value*. So how does a constructor say "the arguments you gave me are invalid, I refuse to build this object"? It can't return `-1`. This is precisely the situation in exercise 00: a `Bureaucrat` born with grade 200 is *nonsense*, and the object must never come into existence.

**Exceptions are the answer.** An exception is a way to abort the current operation and *jump* — possibly across many stack frames — to a place that declared itself willing to handle the problem. The intervening code doesn't need to know or care.

### 1.2 The three keywords

```cpp
throw   // "Something is wrong. Stop here and hurl this object upward."
try     // "I'm going to attempt something that might throw."
catch   // "If it threw this kind of thing, here's how I deal with it."
```

A minimal, generic shape (NOT the project — just the mechanism):

```cpp
void withdraw(int balance, int amount)
{
    if (amount > balance)
        throw std::runtime_error("insufficient funds");
    // ... normal path continues only if we didn't throw
}

int main()
{
    try
    {
        withdraw(100, 500);          // this throws
        std::cout << "done\n";        // <-- SKIPPED. Never runs.
    }
    catch (std::exception & e)
    {
        std::cout << e.what() << "\n"; // prints "insufficient funds"
    }
    // execution calmly continues here, problem handled
}
```

Trace the control flow until it feels obvious:
1. `throw` immediately stops `withdraw`. The line after it does not run.
2. Control unwinds out of `withdraw`, back into the `try` block.
3. The `try` is abandoned mid-flight — `"done\n"` never prints.
4. The runtime looks for a `catch` whose type matches what was thrown.
5. That `catch` runs. Afterward, life goes on *below* the catch.

> **Mental model.** A `throw` is an emergency ejector seat. It launches you straight up through every floor of the building until you reach a floor (`catch`) that's prepared to receive you. Every floor in between is simply skipped — and, crucially, cleaned up properly on the way (next point).

### 1.3 Stack unwinding & why this is safe in C++ (the part that matters)

When an exception flies up through the stack, C++ does something C can't: as it leaves each scope, **it runs the destructors of every fully-constructed local object in that scope.** This is called *stack unwinding*. It's the reason exceptions don't automatically mean leaks — your stack objects clean themselves up.

The catch (pun intended): this safety only covers objects on the *stack*. If you did `new` something and a throw happens before you `delete` it, **that heap allocation leaks** — the pointer was a stack variable, but unwinding only destroys the pointer, not what it points to. The subject explicitly warns you about `new` leaks. In Module 05 you can largely sidestep this by preferring stack objects, but stay alert in exercises 02–03 where you'll allocate forms.

### 1.4 What can you throw? Why a *class*?

You can technically `throw` an `int` or a string. Don't. The idiom — and what the subject asks for — is to throw **objects of a dedicated exception class**, for three reasons:

1. **Type *is* information.** `catch` matches on type. Throwing a `GradeTooHighException` vs a `GradeTooLowException` lets the catcher tell *which* failure happened just by the type it catches, no string parsing.
2. **They form a hierarchy.** This is the keystone. The standard library gives you `std::exception` as a common base class. If your custom exception inherits from it, then `catch (std::exception & e)` will catch *your* exception too (a child is-a parent — Module 04 polymorphism). That's why the subject's example catches `std::exception &`: one catch block handles every well-behaved exception.
3. **`.what()`** — `std::exception` defines a virtual method `virtual const char* what() const throw();` that returns a description. Override it in your exception class to produce your own message.

The required shape of a custom exception (a *nested* class — note the subject writes `Bureaucrat::GradeTooHighException`):

```cpp
class Account
{
public:
    class InsufficientFundsException : public std::exception
    {
    public:
        const char* what() const throw()   // override; note the throw() — see 1.7
        {
            return "not enough money";
        }
    };
    // ...
};
```

Then `throw Account::InsufficientFundsException();` — note the `()`, you're constructing a temporary object. And it's catchable as `catch (std::exception & e)`. You include `<exception>` for the base class, or `<stdexcept>` if you'd rather use ready-made ones like `std::runtime_error` (but the subject wants *named* ones like `GradeTooHighException`, so you'll define your own).

### 1.5 Catch by reference. Always.

```cpp
catch (std::exception & e)   // YES — reference
catch (std::exception e)     // NO  — slices!
```

If you catch *by value*, and someone threw a `GradeTooHighException` (a child), copying it into an `std::exception` variable **slices off** the child part — you lose the overridden `what()` and get the base behavior. This is the classic *object slicing* trap from Module 04, and it bites hardest in exception handling. **Catch by reference** so polymorphism survives. (Catching by `const &` is even more proper.)

### 1.6 Where the throws actually go in this project

Map the mechanism onto the exercises so it's concrete:
- **Constructor** (ex00): validate the grade; if out of range, `throw` — the object is never born. This is the canonical "constructor can't return an error" use case.
- **Mutators** (ex00): `incrementGrade` / `decrementGrade` push toward the limits; if they'd cross 1 or 150, throw the *same* exceptions.
- **A method acting on another object** (ex01): `beSigned(Bureaucrat const &)` inspects the bureaucrat's grade and throws if too low. The *caller* (`signForm`) wraps the call in try/catch and reacts. This is the "objects communicating failure across class boundaries" lesson.
- **Execution checks** (ex02): `execute()` throws if the form isn't signed or the executor's grade is too low.

Notice a design question the subject hints at in ex02: *where* do you put the sign/grade check — in each concrete form, or once in the base class? "One way is more elegant than the other." Think about **don't-repeat-yourself**: if every concrete form needs the same precondition check, that check wants to live in *one* place (the base) which then calls a per-form action hook. Hold that thought until section 3.

### 1.7 The `throw()` curiosity you'll see (C++98-specific)

You'll notice `const char* what() const throw();`. That trailing `throw()` is a **C++98 exception specification** meaning "this function promises to throw nothing." It exists because the base class declares `what()` that way and your override must match. In modern C++ this was replaced by `noexcept` and the `throw()` form is *deprecated* — but you're pinned to **C++98**, so you'll write `throw()`. File this under "things that look weird but are just the dialect."

---

## 2. Quick refreshers: the C++-specific tools you'll lean on

You asked for refreshers only where they're *vital to this module*. Here they are — tight, and chosen because you'll touch each one in M05.

### 2.1 `<<` operator overloading (you'll do this in 00 and 01)

The subject asks you to overload the insertion operator so `std::cout << myBureaucrat` prints nicely. The thing that trips people: this operator **cannot be a member function** of your class, because the left operand is the *stream* (`std::ostream`), not your object. So it's a **free (non-member) function**:

```cpp
std::ostream& operator<<(std::ostream& os, const Account& a)
{
    os << a.getBalance() << " credits";
    return os;          // return the stream so you can chain: cout << a << b << "\n";
}
```

Two non-negotiables: take the stream **by reference**, and **return it** (that's what makes `cout << x << y` chain). If your `<<` needs private data, either go through public getters (cleanest here) or declare it a `friend` — but **the subject forbids `friend`** in these modules, so use getters.

### 2.2 `const` correctness (it's everywhere in the requirements)

This module is strict about `const`, and the subject's wording is a spec:
- **Constant attributes** — the `Bureaucrat`'s name, the `Form`'s name and required grades are `const`. A `const` member can *only* be set in the **constructor initialization list**, never assigned in the body. This forces you to learn init lists properly:
  ```cpp
  Account::Account(std::string n) : _name(n), _id(nextId()) {}  // init list, before the { }
  ```
- **`const` member functions** — `getName() const`, and notably `execute(Bureaucrat const & executor) const`. The trailing `const` promises the method won't modify the object. Getters should all be `const`. If a `const` method tries to mutate a member, it won't compile — that's the language protecting you.
- **`const &` parameters** — passing `Bureaucrat const &` avoids a copy *and* promises you won't modify the caller's object. You'll see this in `beSigned`, `execute`, `signForm`, `executeForm`.

> Why this matters beyond pedantry: a `const` member function can be called on a `const` object; a non-`const` one cannot. `execute(...) const` means even a `const` form can be executed. Getting `const` right is a recurring evaluation point.

### 2.3 The Orthodox Canonical Form (required, a Module 02 callback)

From Modules 02–09 every class (except the exception classes here, the subject says) must provide the **four canonical members**:

```cpp
class Account
{
public:
    Account();                               // 1. default constructor
    Account(const Account& other);           // 2. copy constructor
    Account& operator=(const Account& rhs);  // 3. copy-assignment operator
    ~Account();                              // 4. destructor
};
```

The wrinkle Module 05 forces you to confront: **`const` members can't be reassigned.** So in your copy-assignment `operator=`, you *cannot* copy the `const` name/grades — they were fixed at construction. You'll have to decide what assignment even *means* when half the object is immutable (typically: copy the non-const, mutable state like the "is signed" flag, leave the const identity alone). That tension is intentional and worth sitting with rather than papering over.

### 2.4 Initialization lists & member order (bites you in ex02)

Members are initialized in the **order they're declared in the class**, *not* the order you write them in the init list. With several `const` grades this matters; `-Wall -Wextra` will warn you if your init-list order disagrees with declaration order. Just keep them consistent.

---

## 3. Inheritance, virtual, and abstract classes — the ex02 heart

This is the second big idea, and where Module 04's polymorphism gets a real workout. You asked for depth where vital — this is vital.

### 3.1 The setup: one base, many concrete forms

Exercise 02 renames `Form` to `AForm` and makes it **abstract**. Three concrete forms (`ShrubberyCreationForm`, `RobotomyRequestForm`, `PresidentialPardonForm`) inherit from it. They share everything *structural* (a name, required grades, a signed flag, the precondition checks) but each *does something completely different* when executed. That shape — "same interface, different behavior" — is exactly what inheritance + virtual functions exist for.

### 3.2 `virtual`: the mechanism that makes polymorphism work

A quick but essential refresher, because the whole exercise hinges on it. When you call a method through a base-class pointer or reference:

```cpp
AForm* f = new RobotomyRequestForm("Bender");
f->execute(someBureaucrat);   // which execute() runs?
```

- If `execute` is **not** `virtual`: the compiler picks the function based on the *pointer's* type (`AForm`) — the **static** type. You'd get `AForm`'s version. Wrong — you wanted the robotomy.
- If `execute` **is** `virtual`: the runtime picks based on the *actual object's* type (`RobotomyRequestForm`) — the **dynamic** type. You get the drilling noises. Right.

That runtime dispatch is **polymorphism**, and `virtual` is the switch that turns it on. Without it, a base pointer is blind to what it really points at.

> **Mental model.** `virtual` means "ask the object what it is, don't assume from the label on the box." A non-virtual call trusts the label (the pointer type); a virtual call opens the box (the real object).

### 3.3 Pure virtual = the class becomes abstract

The subject says `AForm` must be **abstract** — you must not be able to create a plain `AForm`, only its concrete children. You achieve this with a **pure virtual function**:

```cpp
class AForm
{
public:
    virtual void executeAction() const = 0;   // the "= 0" makes it PURE virtual
};
```

That `= 0` says "this method has no implementation here; every concrete child *must* provide one." A class with even one pure virtual function is **abstract**: the compiler forbids `new AForm(...)` or `AForm f;`. You can still have `AForm*` and `AForm&` (and you will — that's the whole point: handle all forms through the base type). This is the language enforcing "AForm is just a concept; only real forms exist."

### 3.4 The virtual destructor — do not forget this one

When you delete a child through a base pointer:

```cpp
AForm* f = new ShrubberyCreationForm("home");
delete f;   // calls ~AForm(). Does it also call ~ShrubberyCreationForm()?
```

**Only if `~AForm()` is `virtual`.** If the base destructor isn't virtual, `delete f` runs only the base destructor — the child's destructor is skipped, and any resources the child owned **leak**. The rule: *a class with any virtual function needs a virtual destructor.* You'll have virtual functions all over `AForm`, so:

```cpp
virtual ~AForm();
```

This is a classic evaluation gotcha and a real-world bug magnet. Make it reflex.

### 3.5 The elegant-vs-ugly design choice (the "one way is more elegant" hint)

Back to section 1.6's cliffhanger. Every concrete form's `execute` must first check: *is it signed? is the executor's grade high enough?* — identical logic everywhere. Then it does its unique action.

The inelegant way: copy-paste those checks into all three forms. The elegant way is the **Template Method pattern**:
- The **base** `AForm::execute(Bureaucrat const &) const` does the shared precondition checks (throwing if they fail)...
- ...then calls a **pure virtual** hook (say `executeAction()`) that each concrete form overrides with *only* its unique behavior.

So the base owns the "rules," the children own the "action." Add a fourth form later? You write only its action; the checks come free. That's the design the subject is nudging you toward with "one way is more elegant than the other." You don't *have* to do it this way, but recognizing *why* it's better is half the lesson.

### 3.6 `protected` vs `private` (a subtlety the subject is picky about)

The subject insists the form's attributes stay **private** (not protected) and live in the **base** class. But children need to *use* the name and grades. The resolution: children access them through the base's **public/protected getters**, not by touching the fields directly. This enforces encapsulation even within an inheritance hierarchy — children are clients of the base's interface, not co-owners of its guts. It's a deliberate constraint; respect it rather than reaching for `protected` to make life easy.

---

## 4. The factory pattern — ex03, the victory lap

The `Intern::makeForm(name, target)` returns a `AForm*` chosen by a *string name*. The subject explicitly bans the obvious-but-ugly solution:

```cpp
// FORBIDDEN by the subject — "will not be accepted"
if (name == "shrubbery creation form") return new ShrubberyCreationForm(target);
else if (name == "robotomy request form") return new RobotomyRequestForm(target);
else if (...) ...
```

Why is this bad enough to fail? It scales horribly, repeats itself, and buries the *what* (map a name to a type) under control-flow noise. The lesson is **data-driven dispatch**: represent the mapping as *data* you iterate over, not as branching *code*.

The shape to think toward (structure only — the wiring is yours to figure out):
- An array of `{ name, creator }` pairs, where `creator` is a **pointer to a member function** that news up the right form.
- Loop the array, compare names, call the matching creator. No matching name → print an error, return `NULL`.

Two C++ tools you'll meet here:
- **Pointer-to-member-function** syntax — genuinely weird-looking (`AForm* (Intern::*)(std::string) const`), and one of the few places this module makes you look up exact syntax. That's fine; understanding *why* (a uniform type for "a function that builds a form") matters more than memorizing the punctuation.
- A small **array of structs** as your lookup table. (Recall: no STL containers yet — `<vector>`/`<map>` are forbidden until Module 08. A plain C-style array is exactly right here.)

If a name doesn't exist, the subject wants an explicit error — and you're allocating with `new`, so think about who `delete`s the returned form (the caller does; document that to yourself).

> The deeper takeaway: whenever you catch yourself writing a long if/elseif ladder that maps *values* to *behaviors*, that's a smell. The cure is almost always a table. You'll reuse this instinct for the rest of your career.

---

## 5. The traps that cost people their evaluation

A condensed field guide. Most failed defenses on this module die on one of these:

1. **Catching by value → slicing.** Always `catch (std::exception & e)`. (§1.5)
2. **Non-virtual base destructor → leaks on `delete` through base pointer.** (§3.4)
3. **`new` without `delete`, especially when a throw jumps over the `delete`.** Stack unwinding saves stack objects, *not* heap ones. (§1.3)
4. **Trying to assign `const` members in `operator=`.** Won't compile; rethink what assignment means for a half-immutable object. (§2.3)
5. **Forgetting `what()`'s `const throw()` signature** so it doesn't actually override the base. (§1.4, §1.7)
6. **Off-by-one on grades.** 1 is *highest*, 150 *lowest*. Incrementing improves the grade (3 → 2). "Grade too high" means the *number* went below 1; "too low" means above 150. The inverted scale trips nearly everyone — write it on a sticky note.
7. **Putting function bodies in headers.** The subject says that's an instant 0 (except templates). Declarations in `.hpp`, definitions in `.cpp`.
8. **Missing include guards / headers not self-sufficient.** Each header must compile alone and guard against double inclusion. (Instant 0 territory.)
9. **`using namespace` or `friend`.** Forbidden here — grade -42. Use `std::` prefixes and getters.
10. **Leaving an exception class in Orthodox Canonical Form when the subject excused it** — minor, but shows you read carefully; the subject says exception classes *don't* need OCF.

---

## 6. A suggested learning path (how to actually move)

You asked me to keep you from getting lost in the woods. Here's the trail:

1. **Read the whole module first** (the subject says so, and it's right — ex02 retroactively changes ex01's class into `AForm`). Knowing the destination shapes ex00.
2. **Before coding ex00**, make sure §1 (exceptions) and §2.3 (canonical form with const members) feel solid. If they don't, that's the gap to close — by reading the references below or, even better per the subject, by talking to a neighbor.
3. **Build ex00 as a vehicle for exceptions.** Get one custom exception working end-to-end (throw in constructor → catch in main → print `what()`) before adding the second. One full loop teaches more than two half-built ones.
4. **ex01 is "two objects talking."** Focus on *who throws and who catches*: `beSigned` throws, `signForm` catches. Keep them straight.
5. **ex02 — pause and design before typing.** Decide your base-vs-child split (§3.5) *on paper*. The virtual destructor and the pure virtual action hook are the two load-bearing decisions.
6. **ex03 — resist the if/else.** Sketch the lookup table first. If you find the member-function-pointer syntax fiddly, that's expected; understand the shape, then look up the exact punctuation.
7. **Test like an adversary.** The subject repeatedly says "submit tests." Test the *failure* paths hardest: invalid grades, unsigned execution, too-low executor, unknown form name. The whole module is about failure — your tests should be too.
8. **Rubber-duck your own code** before the defense. If you can't explain *why* the destructor is virtual or *why* you catch by reference, that's exactly what an evaluator (or the exam, with no AI) will expose. Per the subject's own warning: the goal is that *you* can explain it.

---

## 7. Curated resources for going deeper

Use these for the *why* and for exact syntax — not for the project's answers.

**Exceptions**
- [cppreference — Exceptions](https://en.cppreference.com/w/cpp/language/exceptions) — the authoritative mechanics of throw/catch/unwinding.
- [cppreference — std::exception](https://en.cppreference.com/w/cpp/error/exception) — the base class and its `what()`.
- [LearnCpp — Chapter on Exceptions](https://www.learncpp.com/cpp-tutorial/the-need-for-exceptions/) — the best *narrative* explanation, walks the "why" gently. Start here if §1 felt fast.
- [Stack unwinding explained (cppreference RAII)](https://en.cppreference.com/w/cpp/language/raii) — why destructors run during a throw, and why heap allocations are the exception (pun intended).

**Inheritance, virtual, abstract classes**
- [LearnCpp — Virtual functions & polymorphism](https://www.learncpp.com/cpp-tutorial/virtual-functions/) — clearest intuition for static vs dynamic dispatch.
- [LearnCpp — Pure virtual functions, abstract classes](https://www.learncpp.com/cpp-tutorial/pure-virtual-functions-abstract-base-classes-and-interface-classes/) — exactly the `= 0` and abstract-class story for ex02.
- [LearnCpp — Virtual destructors](https://www.learncpp.com/cpp-tutorial/virtual-destructors-virtual-assignment-and-overriding-virtualization/) — read this before ex02, it's trap #2.

**Operator overloading & const**
- [LearnCpp — Overloading `<<`](https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/) — the non-member, return-the-stream pattern from §2.1.
- [cppreference — const member functions](https://en.cppreference.com/w/cpp/language/member_functions) — for §2.2.

**Patterns**
- [Refactoring Guru — Factory Method](https://refactoring.guru/design-patterns/factory-method) — concept behind ex03 (it's heavier than you need, but the *idea* of replacing conditionals with a registry is the gold).
- Search term for the ex02 design: **"Template Method pattern"** — that's the name for "base does the checks, children do the action."

**C++98 specifics**
- When something compiles oddly, remember you're on **C++98** — features like `nullptr`, `auto`, `override`, range-for, and `noexcept` don't exist yet. Use `NULL`, explicit types, and `throw()`. cppreference marks the version each feature arrived in; check it when in doubt.

**42-specific**
- Talk to people who've done Module 05. The subject is emphatic about peer learning, and a 10-minute whiteboard chat about "where do you put the execute checks" will teach you more than an hour of reading. That's not a platitude here — it's literally the graded methodology.

---

### Final word

This module is the first time C++ asks you to think about *failure as a first-class part of your design*, and about *behavior that varies by type behind a common interface*. Those two ideas — exceptions and polymorphism — are load-bearing for everything that comes after, in C++ and well beyond it. Build them properly now, slowly, by hand, and the rest of the Common Core gets easier. Rush them with shortcuts and, as the subject bluntly warns, the exam will find out.

Now go make an artificial nightmare of offices and forms. Have fun with it — the theme is ridiculous on purpose.
