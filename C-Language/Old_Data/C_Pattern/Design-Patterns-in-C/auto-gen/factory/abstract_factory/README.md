Abstract Factory
================

* Creating families of related or dependent products.
* Creating product depending on 2-Dimension or 2-Stage.
* Cooperate with DI (Dependency injection)
* Transfer dependency from IProduct to IProductFactory which controlling the detail creating process

Why Abstract
============

The benefits of abstract:
* Open/closed principle:
  - software entities (classes, modules, functions, etc.) should be **open for extension**,
  - but **closed for modification**
* Dependency inversion principle:
  - High-level modules should not depend on low-level modules. Both should **depend on abstractions**.
    + Hight-level modules donnot care about the concrete instance and where it come from, just the interface
    + The glue code can inject the concrete instance later in the runtime using Construct-Injection, Setter-Injection or Service-Locator etc.
  - Abstractions should not depend on details. Details should depend on abstractions.

Factory Method vs Abstract Factory
===================================

1. Background worker diff
  - AbstractFactory pattern uses composition to delegate responsibility of creating object to another class
  - while Factory design pattern uses inheritance and relies on derived class or sub class to create object.

2. Factory's produce diff
  - With the Factory pattern, you produce implementations (Apple, Banana, Cherry, etc.) of a particular interface -- say, IFruit.
  - With the Abstract Factory pattern, you produce implementations of a particular Factory interface -- e.g., IFruitFactory. Each of those knows how to create different kinds of fruit.

3. Factory's produce diff
  - Factory pattern: The factory produces IProduct-implementations
  - Abstract Factory Pattern: A factory-factory produces IFactories, which in turn produces IProducts :)

Mark Seemann Suggests about DI
==============================

The author of book "Dependency injection in .net", the following suggests come from the really code issues:
* [Init objects through **Constructor Injection**](http://stackoverflow.com/questions/1943576/is-there-a-pattern-for-initializing-objects-created-wth-a-di-container/1945023#1945023)
* [Init objects depend on **2-stage**, using Constructor Injection and Setter-Paramize, Like **Functors**](http://stackoverflow.com/questions/1926826/cant-combine-factory-di/1927167#1927167)
* [factory.Create(profileType).GetProfile(profileName)](http://stackoverflow.com/questions/2168704/wcf-dependency-injection-and-abstract-factory/2168882#2168882)
* [Init objects with **context**](http://stackoverflow.com/questions/1890341/how-to-set-up-ioc-when-a-key-class-needs-session-or-other-context-specific-varia/1890463#1890463)
* [Take a dependency on IServiceFactory instead of IService itself to **hide** create detail](http://stackoverflow.com/questions/2180276/how-to-resolve-type-based-on-end-user-configuration-value/2182775#2182775)
* http://stackoverflow.com/questions/1892532/is-this-the-correct-way-to-use-and-test-a-class-that-makes-use-of-the-factory-pat/1892636#1892636
* [Why factory should be abstract](http://stackoverflow.com/questions/2194629/ddd-book-eric-evans-please-explain-what-is-meant-by-the-factory-should-be-abst/2198504#2198504)
* http://stackoverflow.com/questions/2060767/di-container-factory-or-new-for-ephemeral-objects/2060840#2060840
* http://stackoverflow.com/questions/1900358/how-to-unit-test-instance-creation/1900446#1900446
* http://stackoverflow.com/questions/1682925/what-is-the-best-strategy-for-dependency-injection-of-user-input/1686403#1686403
