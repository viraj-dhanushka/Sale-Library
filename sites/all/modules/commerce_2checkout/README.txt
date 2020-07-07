This module contains basic integration with 2checkout, basically using
it just for credit card integration.

It supports two ways of working with 2checkout:

1. Taking address information all inside Drupal Commerce, redirecting
   to 2checkout only when payment is selected. However 2checkout will
   ask for addres details again.

2. Redirect 2checkout to complete the actual checkout. This is
   probably the mechanism that works best with 2checkout as it seems
   impossible to configure it to just request credit card details.

   In this case all address details are entered in 2checkout, and only
   upon return to your site will Drupal Commerce complete the order,
   taking all the address details from 2checkout.


NOTE: you must have an SSL certificate on your site, else your browser
will complain that information is passed insecurely (which it will be).

TODO:
- Perhaps there is a mechanism to notify 2checkout if you set an order
  to shipping so it's set to shipped there too.
- 2checkout offers some form of recurring billing.
- Coupon integration?



Here a rule that allows redirect to 2checkout upon adding a product to
cart:

{ "rules_redirect_to_2checkout_on_add_to_cart" : {
    "LABEL" : "Redirect to 2checkout on add to cart",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [
      "commerce_shipping",
      "commerce_2checkout",
      "commerce_order",
      "commerce_payment",
      "commerce_cart"
    ],
    "ON" : [ "commerce_cart_product_add" ],
    "DO" : [
      { "commerce_shipping_method_collect_rates" : {
          "shipping_method_name" : "flat_rate",
          "commerce_order" : [ "commerce_order" ]
        }
      },
      { "commerce_shipping_delete_shipping_line_items" : { "commerce_order" : [ "commerce_order" ] } },
      { "commerce_2checkout_add_shipping" : { "commerce_order" : [ "commerce_order" ] } },
      { "commerce_order_update_status" : {
          "commerce_order" : [ "commerce_order" ],
          "order_status" : "checkout_payment"
        }
      },
      { "commerce_payment_enable_2checkout" : {
          "commerce_order" : [ "commerce_order" ],
          "payment_method" : { "value" : {
              "method_id" : "2checkout",
              "settings" : {
                "business" : "1234567890",
                "language" : "en",
                "demo" : 0,
                "skip_landing" : 1,
                "one_page_checkout" : 1,
                "one_line" : 0,
                "tangible" : 1,
                "logging" : ""
              }
            }
          }
        }
      },
      { "commerce_2checkout_redirect" : { "commerce_order" : [ "commerce_order" ] } }
    ]
  }
}
