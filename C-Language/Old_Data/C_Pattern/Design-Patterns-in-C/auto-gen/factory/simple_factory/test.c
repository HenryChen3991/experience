
#include <stdio.h>
#include <stdlib.h>

#include <myobj.h>
#include <test_suite.h>
#include "pizza_store2.h"
#include "pizza_simple_factory.h"

static int test_simple_factory(char *output, size_t sz)
{
	struct pizza *pizza;
	struct pizza_store2 *store;
	struct pizza_simple_factory *factory;
	
	factory = malloc(sizeof(*factory));
	pizza_simple_factory_init(factory);

	store = malloc(sizeof(*store));
	pizza_store2_init(store, factory);

	pizza = pizza_store2_order_pizza(store, "cheese", pizza_size_large);
	pizza_free(pizza);

	pizza = pizza_store2_order_pizza(store, "veggie", pizza_size_normal);
	pizza_free(pizza);

	pizza_store2_free(store);

	return 0;
}

void factory_simple_test(void);
void factory_simple_test(void)
{
	my_test_suite_add(test_simple_factory, "Simple Factory");
}
