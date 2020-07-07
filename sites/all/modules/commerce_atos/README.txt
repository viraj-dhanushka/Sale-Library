CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * User permissions


INTRODUCTION
 ------------

 Current Maintainer:
 * Julien Dubreuil (JulienD) - http://drupal.org/user/519520

 The commerce_atos modules integrates ATOS into the Drupal Commerce payment and
 checkout systems. This module is an off-site payment solution and is builded
 into two sub modules to allow to react on differents features.

    Commerce_atos_payment:
    - Direct payment, allow to capture the amount when the customer completes
     the checkout process.

    - Authorization only, allow transactions to be captured later. The capture
    will be automated after several days (This period can be setted through
    the admin).

    Commerce_atos_instalment
    - Instalment. This payment method allow the customer to pay in several times.


REQUIREMENTS
------------

 Having an ATOS account


INSTALLATION
------------

 1. Download and extract the module's tarball (*.tar.gz archive file) into
    your Drupal site's contributed/custom modules directory:

    /sites/all/modules

 2. Go to the site's module page:

    Administration > Modules

 3. Select the module you want to enable (either commerce_atos_instalment,
    either commerce_atos_instalmentthe or both):

        Administration > Modules

 4. Configure the payment method on the payment methods lists:

    Administration > Store settings > Advanced store settings > Payment methods

    Click edit on the rules titled "ATOS SIPS ...".

    Click on the edit button located in the "Action" area at the bottom of the
    page to access to the payment configuration.

    Fill out the form with your credentials under "Payment settings" and save
    the form.

 4. Enable the payment method by clicking the enable link on the payment method
    overview

    Administration > Store settings > Advanced store settings > Payment methods


USER PERMISSIONS
----------------

 The module provides user/role permission which can be granted at:

      Administration > People > Permissions
