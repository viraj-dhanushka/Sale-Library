<?php

namespace MailjetTools;

use Exception;
use Mailjet\Client;
use Mailjet\Resources;
use MailjetTools\MailjetIframe;

class MailjetApi
{
    private static $mjApiClient = null;
    public static $mjApiKey = null;

    public static function getApiClient($mailjetApikey = null, $mailjetApiSecret = null)
    {
        if (self::$mjApiClient instanceof Client) {
            return self::$mjApiClient;
        }
        if (empty($mailjetApikey) || empty($mailjetApiSecret)) {
            throw new \Exception('Missing Mailjet API credentials');
        }

        $mjClient = new Client($mailjetApikey, $mailjetApiSecret);
        if (drupal_get_profile() == 'standard') {
            $mjClient->addRequestOption(CURLOPT_USERAGENT, 'drupal-7');
            $mjClient->addRequestOption('headers', ['User-Agent' => 'drupal-7']);
        } elseif (drupal_get_profile() == 'commerce_kickstart') {
            $mjClient->addRequestOption(CURLOPT_USERAGENT, 'kickstart');
            $mjClient->addRequestOption('headers', ['User-Agent' => 'kickstart']);

        } else {
            $mjClient->addRequestOption(CURLOPT_USERAGENT, 'drupal-7');
            $mjClient->addRequestOption('headers', ['User-Agent' => 'drupal-7']);
        }

        // We turn of secure protocol for API requests if the wordpress does not support it
        if (empty($_SERVER['HTTPS']) || (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'off') || $_SERVER['SERVER_PORT'] != 443) {
            $mjClient->setSecureProtocol(false);
        }
        self::$mjApiKey = $mailjetApikey;
        self::$mjApiClient = $mjClient;

        return self::$mjApiClient;
    }


    public static function getMailjetContactLists($limit = 0)
    {
        $mjApiClient = self::getApiClient();

        $filters = [
            'Limit' => $limit,
            'Sort' => 'Name ASC'
        ];
        $response = $mjApiClient->get(Resources::$Contactslist, ['filters' => $filters]);
        if ($response->success()) {
            return $response->getData();
        } else {
            //return $response->getStatus();
            return false;
        }
    }

    public static function getMailjetContactListByName($name)
    {
        $mjApiClient = self::getApiClient();

        $filters = [
            'Name' => $name
        ];
        $response = $mjApiClient->get(Resources::$Contactslist, ['filters' => $filters]);
        if ($response->success() && $response->getCount() > 0) {
            return $response->getData();
        } else {
            //return $response->getStatus();
            return false;
        }
    }


    public static function getMailjetContactProperties($limit = 0)
    {
        $mjApiClient = self::getApiClient();

        $filters = [
            'Limit' => $limit
        ];
        $response = $mjApiClient->get(Resources::$Contactmetadata, ['filters' => $filters]);
        if ($response->success()) {
            return $response->getData();
        } else {
            //return $response->getStatus();
            return false;
        }
    }

    public static function createMailjetContactList($listName)
    {
        if (empty($listName)) {
            return false;
        }

        $mjApiClient = self::getApiClient();

        $body = [
            'Name' => $listName
        ];
        $response = $mjApiClient->post(Resources::$Contactslist, ['body' => $body]);
        if ($response->success()) {
            return $response->getData();
        } else {
            //return $response->getStatus();
            return false;
        }
    }

    public static function isContactListActive($contactListId)
    {
        if (!$contactListId) {
            return false;
        }
        try {
            $mjApiClient = self::getApiClient();
        } catch (\Exception $e) {
            return false;
        }
        $filters = array(
            'ID' => $contactListId
        );
        $response = $mjApiClient->get(Resources::$Contactslist, array('filters' => $filters));
        if ($response->success()) {
            $data = $response->getData();
            if (isset($data[0]['IsDeleted'])) {
                // Return true if the list is not deleted
                return !$data[0]['IsDeleted'];
            }
        }
        return false;
    }

    public static function getContactProperties()
    {
        $mjApiClient = self::getApiClient();
        $filters = array(
            'limit' => 0,
            'Sort' => 'Name ASC'
        );
        $response = $mjApiClient->get(Resources::$Contactmetadata, array('filters' => $filters));
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
//            return $response->getStatus();
        }
    }

    public static function getPropertyIdByName($name)
    {
        if (!$name) {
           return false; 
        }
        $contactProperties = self::getContactProperties();
        if ($contactProperties) {
            foreach ($contactProperties as $property) {
                if ($property['Name'] === $name) {
                    return $property['ID'];
                }
            }
        }
        return false;
    }

    public static function createMailjetContactProperty($name, $type = "str")
    {
        if (empty($name)) {
            return false;
        }

        $mjApiClient = self::getApiClient();

//      Name: the name of the custom data field
//      DataType: the type of data that is being stored (this can be either a str, int, float or bool)
//      NameSpace: this can be either static or historic
        $body = [
            'Datatype' => $type,
            'Name' => $name,
            'NameSpace' => "static"
        ];
        $response = $mjApiClient->post(Resources::$Contactmetadata, ['body' => $body]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
//            return $response->getStatus();
        }
    }


    public static function updateMailjetContactProperty($id, $name, $type = "str")
    {
        if (empty($name)) {
            return false;
        }

        $mjApiClient = self::getApiClient();

//      Name: the name of the custom data field
//      DataType: the type of data that is being stored (this can be either a str, int, float or bool)
//      NameSpace: this can be either static or historic
        $body = [
            'Datatype' => $type,
            'Name' => $name,
            'NameSpace' => "static"
        ];
        $response = $mjApiClient->put(Resources::$Contactmetadata, ['id' => $id, 'body' => $body]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
//            return $response->getStatus();
        }
    }

    public static function getMailjetSenders()
    {
        $mjApiClient = self::getApiClient();

        $filters = [
            'Limit' => '0',
            'Sort' => 'ID DESC'
        ];

        $response = $mjApiClient->get(Resources::$Sender, ['filters' => $filters]);
        if ($response->success()) {
            return $response->getData();
        } else {
            //return $response->getStatus();
            return false;
        }
    }

    public static function isValidAPICredentials()
    {
        try {
            $mjApiClient = self::getApiClient();
        } catch (\Exception $e) {
            return false;
        }

        $filters = [
            'Limit' => '1'
        ];

        $response = $mjApiClient->get(Resources::$Contactmetadata, ['filters' => $filters]);
        if ($response->success()) {
            return true;
            // return $response->getData();
        } else {
            return false;
            // return $response->getStatus();
        }
    }

    /**
     * Add or Remove a contact to a Mailjet contact list - It can process many or single contact at once
     *
     * @param $contactListId - int - ID of the contact list to sync contacts
     * @param $contacts - array('Email' => ContactEmail, 'Name' => ContactName, 'Properties' => array(propertyName1 => propertyValue1, ...));
     * @param string $action - 'addforce', 'adnoforce', 'remove'
     * @return array|bool
     */
    public static function syncMailjetContacts($contactListId, $contacts, $action = 'addforce')
    {
        $mjApiClient = self::getApiClient();

        $body = [
            'Action' => $action,
            'Contacts' => $contacts
        ];

        $response = $mjApiClient->post(Resources::$ContactslistManagemanycontacts, ['id' => $contactListId, 'body' => $body]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
//            return $response->getStatus();
        }
    }

    /**
     * Add a contact to a Mailjet contact list
     */
    public static function syncMailjetContact($contactListId, $contact, $action = 'addforce')
    {
        $mjApiClient = self::getApiClient();
        $body = [
            'Action' => $action,
            'Email' => $contact['Email'],
        ];
        if (!empty($contact['Properties'])) {
            $body['Properties'] = $contact['Properties'];
        }
        $response = $mjApiClient->post(Resources::$ContactslistManagecontact, ['id' => $contactListId, 'body' => $body]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
        }
    }


    public static function createApiToken(array $params)
    {
        $mjApiClient = self::getApiClient();
        $response = $mjApiClient->post(Resources::$Apitoken, ['body' => $params]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
        }
    }

    public static function getApiToken(string $id)
    {
        $mjApiClient = self::getApiClient();
        $response = $mjApiClient->get(Resources::$Apitoken, ['id' => $id]);
        if ($response->success()) {
            return $response->getData();
        } else {
            return false;
        }
    }



    public static function getMailjetIframe($username, $password)
    {
      $mailjetIframe = new MailjetIframe($username, $password, false);

      $mailjetIframe
        ->setCallback('')
        ->setTokenExpiration(600)
       ->setTokenAccess(array(
          'campaigns',
          'contacts',
          'stats',
        ))
        ->turnDocumentationProperties(MailjetIframe::OFF)
        ->turnNewContactListCreation(MailjetIframe::ON)
        ->turnMenu(MailjetIframe::OFF)
        ->turnFooter(MailjetIframe::ON)
        ->turnBar(MailjetIframe::ON)
        ->turnCreateCampaignButton(MailjetIframe::ON)
        ->turnSendingPolicy(MailjetIframe::ON);

      return $mailjetIframe;
    }


}
