#include "framework.h"


namespace hi5
{


   string prepare_basic_authentication(sp(base_application) papp, const char * inParam1, const char * inParam2);


   namespace oAuthTwitterApiUrls
   {
      /* Twitter OAuth API URLs */
   };

   /* Default values used in twitcurl */
   namespace twit_defaults
   {
   };


   /* Default twitter URLs */
   namespace twitter_defaults
   {


   };


   /*++
   * @method: facebook::facebook
   *
   * @description: constructor
   *
   * @input: none
   *
   * @output: none
   *
   *--*/
   facebook::facebook(sp(base_application) papp) :
      element(papp),

      /* Constants */
      TWIT_COLON(":"),
      TWIT_EOS('\0'),

      /* Miscellaneous data used to build twitter URLs*/
      TWIT_SEARCHQUERYSTRING("?q("),
      TWIT_SCREENNAME("?screen_name("),
      TWIT_USERID("?user_id("),
      TWIT_EXTENSIONFORMAT(".xml"),
      TWIT_TARGETSCREENNAME("?target_screen_name("),
      TWIT_TARGETUSERID("?target_id("),

      /* Search URLs */
      TWIT_SEARCH_URL("http://search.twitter.com/search.atom"),

      /* Status URLs */
      TWIT_STATUSUPDATE_URL("http://twitter.com/statuses/update.xml"),
      TWIT_STATUSSHOW_URL("http://twitter.com/statuses/show/"),
      TWIT_STATUDESTROY_URL("http://twitter.com/statuses/destroy/"),

      /* Timeline URLs */
      TWIT_PUBLIC_TIMELINE_URL("http://twitter.com/statuses/public_timeline.xml"),
      TWIT_FEATURED_USERS_URL("http://twitter.com/statuses/featured.xml"),
      TWIT_FRIENDS_TIMELINE_URL("http://twitter.com/statuses/friends_timeline.xml"),
      TWIT_MENTIONS_URL("http://twitter.com/statuses/mentions.xml"),
      TWIT_USERTIMELINE_URL("http://twitter.com/statuses/user_timeline.xml"),

      /* Users URLs */
      TWIT_SHOWUSERS_URL("http://twitter.com/users/show.xml"),
      TWIT_SHOWFRIENDS_URL("http://twitter.com/statuses/friends.xml"),
      TWIT_SHOWFOLLOWERS_URL("http://twitter.com/statuses/followers.xml"),

      /* Direct messages URLs */
      TWIT_DIRECTMESSAGES_URL("http://twitter.com/direct_messages.xml"),
      TWIT_DIRECTMESSAGENEW_URL("http://twitter.com/direct_messages/new.xml"),
      TWIT_DIRECTMESSAGESSENT_URL("http://twitter.com/direct_messages/sent.xml"),
      TWIT_DIRECTMESSAGEDESTROY_URL("http://twitter.com/direct_messages/destroy/"),

      /* Friendships URLs */
      TWIT_FRIENDSHIPSCREATE_URL("http://twitter.com/friendships/create.xml"),
      TWIT_FRIENDSHIPSDESTROY_URL("http://twitter.com/friendships/destroy.xml"),
      TWIT_FRIENDSHIPSSHOW_URL("http://twitter.com/friendships/show.xml"),

      /* Social graphs URLs */
      TWIT_FRIENDSIDS_URL("http://twitter.com/friends/ids.xml"),
      TWIT_FOLLOWERSIDS_URL("http://twitter.com/followers/ids.xml"),

      /* Ac::count URLs */
      TWIT_ACCOUNTRATELIMIT_URL("http://twitter.com/account/rate_limit_status.xml"),

      /* Favorites URLs */
      TWIT_FAVORITESGET_URL("http://twitter.com/favorites.xml"),
      TWIT_FAVORITECREATE_URL("http://twitter.com/favorites/create/"),
      TWIT_FAVORITEDESTROY_URL("http://twitter.com/favorites/destroy/"),

      /* Block URLs */
      TWIT_BLOCKSCREATE_URL("http://twitter.com/blocks/create/"),
      TWIT_BLOCKSDESTROY_URL("http://twitter.com/blocks/destroy/"),

      /* Saved Search URLs */
      TWIT_SAVEDSEARCHGET_URL("http://twitter.com/saved_searches.xml"),
      TWIT_SAVEDSEARCHSHOW_URL("http://twitter.com/saved_searches/show/"),
      TWIT_SAVEDSEARCHCREATE_URL("http://twitter.com/saved_searches/create.xml"),
      TWIT_SAVEDSEARCHDESTROY_URL("http://twitter.com/saved_searches/destroy/"),

      /* Trends URLs */
      TWIT_TRENDS_URL("http://api.twitter.com/1/trends.json"),
      TWIT_TRENDSDAILY_URL("http://api.twitter.com/1/trends/daily.json"),
      TWIT_TRENDSCURRENT_URL("http://api.twitter.com/1/trends/current.json"),
      TWIT_TRENDSWEEKLY_URL("http://api.twitter.com/1/trends/weekly.json"),
      TWIT_TRENDSAVAILABLE_URL("http://api.twitter.com/1/trends/available.json"),
      m_oauth(papp)
   {

      m_setHttp["raw_http"] = true;

      m_setHttp["minimal_headers"] = true;

      m_curlLoginParamsSet = false;

   }

   /*++
   * @method: facebook::~facebook
   *
   * @description: destructor
   *
   * @input: none
   *
   * @output: none
   *
   *--*/
   facebook::~facebook()
   {
   }

   /*++
   * @method: facebook::getTwitterUsername
   *
   * @description: method to get stored Twitter username
   *
   * @input: none
   *
   * @output: twitter username
   *
   *--*/
   string & facebook::getTwitterUsername()
   {
      return m_twitterUsername;
   }

   /*++
   * @method: facebook::getTwitterPassword
   *
   * @description: method to get stored Twitter password
   *
   * @input: none
   *
   * @output: twitter password
   *
   *--*/
   string & facebook::getTwitterPassword()
   {
      return m_twitterPassword;
   }

   /*++
   * @method: facebook::setTwitterUsername
   *
   * @description: method to set username
   *
   * @input: userName
   *
   * @output: none
   *
   *--*/
   void facebook::setTwitterUsername(string & userName)
   {
      if (userName.get_length())
      {
         m_twitterUsername = userName;
         m_curlLoginParamsSet = false;
      }
   }

   /*++
   * @method: facebook::setTwitterPassword
   *
   * @description: method to set password
   *
   * @input: passWord
   *
   * @output: none
   *
   *--*/
   void facebook::setTwitterPassword(string & passWord)
   {
      if (passWord.get_length())
      {
         m_twitterPassword = passWord;
         m_curlLoginParamsSet = false;
      }
   }


   /*++
   * @method: facebook::search
   *
   * @description: method to return tweets that match a specified query.
   *
   * @input: query - search query in string format
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::search(string & query)
   {
      bool retVal = false;
      /* Prepare URL */
      string strUrl("");
      strUrl = TWIT_SEARCH_URL;
      strUrl.append(TWIT_SEARCHQUERYSTRING);
      strUrl.append(query);

      /* Perform GET */
      retVal = performGet(strUrl);
      return retVal;
   }

   /*++
   * @method: facebook::statusUpdate
   *
   * @description: method to update new status message in twitter profile
   *
   * @input: newStatus
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::statusUpdate(string & newStatus)
   {
      bool retVal = false;
      if (true && newStatus.has_char())
      {
         /* Prepare new status message */
         property_set post(get_app());
         post["status"] = newStatus;

         /* Perform POST */
         retVal = performPost("https://api.twitter.com/1.1/statuses/update.json", post);
      }
      return retVal;
   }

   /*++
   * @method: facebook::statusShowById
   *
   * @description: method to get a status message by its id
   *
   * @input: statusId - a number in string format
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::statusShowById(string & statusId)
   {
      bool retVal = false;
      if (true && statusId.get_length())
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_STATUSSHOW_URL;
         strUrl.append(statusId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::statusDestroyById
   *
   * @description: method to delete a status message by its id
   *
   * @input: statusId - a number in string format
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::statusDestroyById(string & statusId)
   {
      bool retVal = false;
      if (true && statusId.get_length())
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_STATUDESTROY_URL;
         strUrl.append(statusId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::timelinePublicGet
   *
   * @description: method to get public timeline
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::timelinePublicGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_PUBLIC_TIMELINE_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::featuredUsersGet
   *
   * @description: method to get featured users
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::featuredUsersGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_FEATURED_USERS_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::timelineFriendsGet
   *
   * @description: method to get friends timeline
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::timelineFriendsGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_FRIENDS_TIMELINE_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::mentionsGet
   *
   * @description: method to get mentions
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::mentionsGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_MENTIONS_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::timelineUserGet
   *
   * @description: method to get mentions
   *
   * @input: userInfo - screen name or user id in string format,
   *         isUserId - true if userInfo contains an id
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::timelineUserGet(string userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_USERTIMELINE_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::userGet
   *
   * @description: method to get a user's profile
   *
   * @input: userInfo - screen name or user id in string format,
   *         isUserId - true if userInfo contains an id
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::userGet(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true && userInfo.get_length())
      {
         /* set URL */
         string strUrl("");
         strUrl = build_url(TWIT_SHOWUSERS_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::friendsGet
   *
   * @description: method to get a user's friends
   *
   * @input: userInfo - screen name or user id in string format,
   *         isUserId - true if userInfo contains an id
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::friendsGet(string userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* set URL */
         string strUrl("");
         strUrl = build_url(TWIT_SHOWFRIENDS_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::followersGet
   *
   * @description: method to get a user's followers
   *
   * @input: userInfo - screen name or user id in string format,
   *         isUserId - true if userInfo contains an id
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::followersGet(string userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_SHOWFOLLOWERS_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::directMessageGet
   *
   * @description: method to get direct messages
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::directMessageGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_DIRECTMESSAGES_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::directMessageSend
   *
   * @description: method to send direct message to a user
   *
   * @input: userInfo - screen name or user id of a user to whom message needs to be sent,
   *         dMsg - message
   *         isUserId - true if userInfo contains target user's id
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::directMessageSend(string & userInfo, string & dMsg, bool isUserId)
   {
      bool retVal = false;
      if (true && userInfo.get_length() && dMsg.get_length())
      {
         /* Prepare new direct message */
         property_set post(get_app());
         post["text"] = dMsg;

         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_DIRECTMESSAGENEW_URL, userInfo, isUserId);

         /* Perform POST */
         retVal = performPost(strUrl, post);
      }
      return retVal;
   }

   /*++
   * @method: facebook::directMessageGetSent
   *
   * @description: method to get sent direct messages
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::directMessageGetSent()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_DIRECTMESSAGESSENT_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::directMessageDestroyById
   *
   * @description: method to delete direct messages by its id
   *
   * @input: dMsgId - id of direct message in string format
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::directMessageDestroyById(string & dMsgId)
   {
      bool retVal = false;
      if (true && dMsgId.get_length())
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_DIRECTMESSAGEDESTROY_URL;
         strUrl.append(dMsgId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::friendshipCreate
   *
   * @description: method to add a twitter user as friend (follow a user)
   *
   * @input: userInfo - user id or screen name of a user
   *         isUserId - true if userInfo contains a user id instead of screen name
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::friendshipCreate(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true && userInfo.get_length())
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_FRIENDSHIPSCREATE_URL, userInfo, isUserId);

         /* Send some dummy data in POST */
         property_set post(get_app());
         post["text"] = "dummy";

         /* Perform POST */
         retVal = performPost(strUrl, post);
      }
      return retVal;
   }

   /*++
   * @method: facebook::friendshipDestroy
   *
   * @description: method to delete a twitter user from friend list (unfollow a user)
   *
   * @input: userInfo - user id or screen name of a user
   *         isUserId - true if userInfo contains a user id instead of screen name
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::friendshipDestroy(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true && userInfo.get_length())
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_FRIENDSHIPSDESTROY_URL, userInfo, isUserId);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::friendshipShow
   *
   * @description: method to show all friends
   *
   * @input: userInfo - user id or screen name of a user of whom friends need to be shown
   *         isUserId - true if userInfo contains a user id instead of screen name
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::friendshipShow(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_FRIENDSHIPSSHOW_URL;
         if (userInfo.get_length())
         {
            /* Append username to the URL */
            if (isUserId)
            {
               strUrl.append(TWIT_TARGETUSERID);
            }
            else
            {
               strUrl.append(TWIT_TARGETSCREENNAME);
            }
            strUrl.append(userInfo);
         }

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::friendsIdsGet
   *
   * @description: method to show IDs of all friends of a twitter user
   *
   * @input: userInfo - user id or screen name of a user
   *         isUserId - true if userInfo contains a user id instead of screen name
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::friendsIdsGet(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_FRIENDSIDS_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::followersIdsGet
   *
   * @description: method to show IDs of all followers of a twitter user
   *
   * @input: userInfo - user id or screen name of a user
   *         isUserId - true if userInfo contains a user id instead of screen name
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::followersIdsGet(string & userInfo, bool isUserId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = build_url(TWIT_FOLLOWERSIDS_URL, userInfo, isUserId);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::accountRateLimitGet
   *
   * @description: method to get API rate limit of current user
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::accountRateLimitGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_ACCOUNTRATELIMIT_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::favoriteGet
   *
   * @description: method to get favorite users' statuses
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::favoriteGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_FAVORITESGET_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::favoriteCreate
   *
   * @description: method to favorite a status message
   *
   * @input: statusId - id in string format of the status to be favorited
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::favoriteCreate(string & statusId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_FAVORITECREATE_URL;
         strUrl.append(statusId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Send some dummy data in POST */
         property_set post(get_app());
         post["text"] = "dummy";

         /* Perform POST */
         retVal = performPost(strUrl, post);
      }
      return retVal;
   }

   /*++
   * @method: facebook::favoriteDestroy
   *
   * @description: method to delete a favorited the status
   *
   * @input: statusId - id in string format of the favorite status to be deleted
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::favoriteDestroy(string & statusId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_FAVORITEDESTROY_URL;
         strUrl.append(statusId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::blockCreate
   *
   * @description: method to block a user
   *
   * @input: userInfo - user id or screen name
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::blockCreate(string & userInfo)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_BLOCKSCREATE_URL;
         strUrl.append(userInfo);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Send some dummy data in POST */
         property_set post(get_app());
         post["text"] = "dummy";

         /* Perform POST */
         retVal = performPost(strUrl, post);
      }
      return retVal;
   }

   /*++
   * @method: facebook::blockDestroy
   *
   * @description: method to unblock a user
   *
   * @input: userInfo - user id or screen name
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::blockDestroy(string & userInfo)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_BLOCKSDESTROY_URL;
         strUrl.append(userInfo);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::savedSearchGet
   *
   * @description: gets authenticated user's saved search queries.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::savedSearchGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_SAVEDSEARCHGET_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::savedSearchShow
   *
   * @description: method to retrieve the data for a saved search owned by the authenticating user
   *               specified by the given id.
   *
   * @input: searchId - id in string format of the search to be displayed
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::savedSearchShow(string & searchId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_SAVEDSEARCHSHOW_URL;
         strUrl.append(searchId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform GET */
         retVal = performGet(strUrl);
      }
      return retVal;
   }

   /*++
   * @method: facebook::savedSearchCreate
   *
   * @description: creates a saved search for the authenticated user
   *
   * @input: query - the query of the search the user would like to save
   *
   * @output: true if POST is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::savedSearchCreate(string & query)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_SAVEDSEARCHCREATE_URL;

         /* Send some dummy data in POST */
         property_set post(get_app());
         post["query"] = query;

         /* Perform POST */
         retVal = performPost(strUrl, post);
      }
      return retVal;
   }


   /*++
   * @method: facebook::savedSearchDestroy
   *
   * @description: method to destroy a saved search for the authenticated user. The search specified
   *               by id must be owned by the authenticating user.
   *
   * @input: searchId - search id of item to be deleted
   *
   * @output: true if DELETE is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::savedSearchDestroy(string & searchId)
   {
      bool retVal = false;
      if (true)
      {
         /* Prepare URL */
         string strUrl("");
         strUrl = TWIT_SAVEDSEARCHDESTROY_URL;
         strUrl.append(searchId);
         strUrl.append(TWIT_EXTENSIONFORMAT);

         /* Perform DELETE */
         retVal = performDelete(strUrl);
      }
      return retVal;
   }


   /*++
   * @method: facebook::trendsGet()
   *
   * @description: gets trends.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::trendsGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_TRENDS_URL);
      }
      return retVal;
   }


   /*++
   * @method: facebook::trendsDailyGet()
   *
   * @description: gets daily trends.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::trendsDailyGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_TRENDSDAILY_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::trendsWeeklyGet()
   *
   * @description: gets weekly trends.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::trendsWeeklyGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_TRENDSWEEKLY_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::trendsCurrentGet()
   *
   * @description: gets current trends.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::trendsCurrentGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_TRENDSCURRENT_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::trendsAvailableGet()
   *
   * @description: gets available trends.
   *
   * @input: none
   *
   * @output: true if GET is success, otherwise false. This does not check http
   *          response by twitter. Use get_response() for that.
   *
   *--*/
   bool facebook::trendsAvailableGet()
   {
      bool retVal = false;
      if (true)
      {
         /* Perform GET */
         retVal = performGet(TWIT_TRENDSAVAILABLE_URL);
      }
      return retVal;
   }

   /*++
   * @method: facebook::get_response
   *
   * @description: method to get http response for the most recent request sent.
   *               twitcurl users need to call this method and parse the XML
   *               data returned by twitter to see what has happened.
   *
   * @input: outWebResp - string in which twitter's response is supplied back to caller
   *
   * @output: none
   *
   *--*/
   string facebook::get_response()
   {
      return m_strResponse;
   }



   /*++
   * @method: facebook::prepareCurlUserPass
   *
   * @description: method to set twitter credentials into cURL. this is an internal method.
   *               twitcurl users should not use this method, instead use setTwitterXxx
   *               methods to set twitter username and password.
   *
   * @input: none
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   void facebook::prepareCurlUserPass()
   {

      if (!m_curlLoginParamsSet)
      {

         /* set username and password */
         m_setHttp["basic_authentication"] = prepare_basic_authentication(getTwitterUsername(), getTwitterPassword());

         /* set the flag to true indicating that twitter credentials are set in cURL */
         m_curlLoginParamsSet = true;

      }

   }

   /*++
   * @method: facebook::prepareStandardParams
   *
   * @description: method to set standard params into cURL. this is an internal method.
   *               twitcurl users should not use this method.
   *
   * @input: none
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   void facebook::prepareStandardParams()
   {
      /* Restore any custom request we may have */
      //       curl_easy_setopt( m_curlHandle, CURLOPT_CUSTOMREQUEST, NULL );


      /* Prepare username and password for twitter */
      prepareCurlUserPass();
   }

   /*++
   * @method: facebook::performGet
   *
   * @description: method to send http GET request. this is an internal method.
   *               twitcurl users should not use this method.
   *
   * @input: getUrl - url
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   bool facebook::performGet(const string & getUrl)
   {

      property_set set(get_app());

      /* set OAuth header */
      m_oauth.getOAuthHeader(eOAuthHttpGet, getUrl, set);

      set.merge(m_setHttp);

      /* Send http request */
      return Application.http().get(getUrl, m_strResponse, set);

   }


   /*++
   * @method: facebook::performGet
   *
   * @description: method to send http GET request. this is an internal method.
   *               twitcurl users should not use this method.
   *
   * @input: const string & getUrl, const string & oAuthHttpHeader
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   bool facebook::performGet(const string & getUrl, property_set & headers)
   {

      string dataStrDummy("");

      property_set set(m_setHttp);

      set["headers"] = headers;

      /* Send http request */
      return Application.http().get(getUrl, m_strResponse, set);

   }

   /*   bool facebook::performPost( const string & getUrl, property_set & headers, property_set & post)
   {

   string dataStrDummy( "" );

   property_set set(m_setHttp);

   if (headers.m_propertya.get_count() > 0)
   {

   set["headers"] = headers;

   }

   set["post"] = post;

   set["http_method"] = "POST";

   /* Send http request */
   /*bool bOk = Application.http().get(getUrl, m_strResponse, set);

   headers = set["get_headers"].propset();

   return bOk;

   }*/

   /*++
   * @method: facebook::performDelete
   *
   * @description: method to send http DELETE request. this is an internal method.
   *               twitcurl users should not use this method.
   *
   * @input: deleteUrl - url
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   bool facebook::performDelete(const string & deleteUrl)
   {

      property_set set(get_app());

      /* set OAuth header */
      m_oauth.getOAuthHeader(eOAuthHttpDelete, deleteUrl, set);

      set.merge(m_setHttp);

      set["http_method"] = "DELETE";

      /* Send http request */
      return Application.http().get(deleteUrl, m_strResponse, set);

   }

   /*++
   * @method: facebook::performPost
   *
   * @description: method to send http POST request. this is an internal method.
   *               twitcurl users should not use this method.
   *
   * @input: postUrl - url,
   *         dataStr - data to be posted
   *
   * @output: none
   *
   * @remarks: internal method
   *
   *--*/
   bool facebook::performPost(const string & postUrl, property_set & set)
   {

      if (set.has_property("headers"))
      {

         set.merge(m_setHttp);

         if (set.has_property("post"))
         {

            set["http_method"] = "POST";

         }

         return Application.http().get(postUrl, m_strResponse, set);

      }
      else
      {

         property_set setHttp(get_app());

         setHttp["post"] = set;

         /* set OAuth header */
         m_oauth.getOAuthHeader(eOAuthHttpPost, postUrl, setHttp);

         setHttp.merge(m_setHttp);

         setHttp["http_method"] = "POST";

         return Application.http().get(postUrl, m_strResponse, setHttp) && setHttp["get_status"].int32() == ::http::status_ok;

      }

   }

   /*++
   * @method: utilMakeCurlParams
   *
   * @description: utility function to build parameter strings in the format
   *               required by cURL ("param1:param2"). twitcurl users should
   *               not use this function.
   *
   * @input: inParam1 - first parameter,
   *         inParam2 - second parameter
   *
   * @output: outStr - built parameter
   *
   * @remarks: internal method
   *
   *--*/
   string facebook::prepare_basic_authentication(const char * inParam1, const char * inParam2)
   {
      string outStr = inParam1;
      outStr += TWIT_COLON;
      outStr += inParam2;
      return outStr;
   }

   /*++
   * @method: utilMakeUrlForUser
   *
   * @description: utility function to build url compatible to twitter. twitcurl
   *               users should not use this function.
   *
   * @input: baseUrl - base twitter url,
   *         userInfo - user name,
   *         isUserId - indicates if userInfo contains a user id or scree name
   *
   * @output: outUrl - built url
   *
   * @remarks: internal method
   *
   *--*/
   string facebook::build_url(const char * baseUrl, const char * userInfo, bool isUserId)
   {
      /* Copy base URL */
      string outUrl = baseUrl;

      if (userInfo != NULL && strlen(userInfo)  > 0)
      {
         /* Append username to the URL */
         if (isUserId)
         {
            outUrl += TWIT_USERID;
         }
         else
         {
            outUrl += TWIT_SCREENNAME;
         }
         outUrl += userInfo;
      }
      return outUrl;
   }

   /*++
   * @method: facebook::getOAuth
   *
   * @description: method to get a reference to oAuth object.
   *
   * @input: none
   *
   * @output: reference to oAuth object
   *
   *--*/
   oauth & facebook::get_oauth()
   {
      return m_oauth;
   }

   /*++
   * @method: facebook::oAuthRequestToken
   *
   * @description: method to get a request token key and secret. this token
   *               will be used to get authorize user and get PIN from twitter
   *
   * @input: authorizeUrl is an output parameter. this method will set the url
   *         in this string. user should visit this link and get PIN from that page.
   *
   * @output: true if everything went sucessfully, otherwise false
   *
   *--*/
   bool facebook::oAuthRequestToken(string & authorizeUrl /* out */)
   {
      bool retVal = false;

      authorizeUrl = "";

      if (true)
      {
         /* Get OAuth header for request token */
         property_set set(get_app());
         int64_t iTime = ::time(NULL);
         if (m_oauth.getOAuthHeader(eOAuthHttpPost, "https://twitter.com/oauth/request_token", set))
         {
            if (performPost("https://twitter.com/oauth/request_token", set))
            {
               string strAuth = set["get_headers"]["Authorization"];
               string strDate = set["get_headers"]["date"];
               ::datetime::time time(iTime);
               string strDateHere = System.datetime().international().get_gmt_date_time(time);
               /* Tell OAuth object to save access token and secret from web response */
               m_oauth.extractOAuthTokenKeySecret(get_response());

               /* Get access token and secret from OAuth object */
               string oAuthTokenKey("");
               m_oauth.getOAuthTokenKey(oAuthTokenKey);

               /* Build authorize url so that user can visit in browser and get PIN */
               authorizeUrl = m_oauth.OAUTHLIB_TWITTER_AUTHORIZE_URL;
               authorizeUrl += oAuthTokenKey;

               retVal = true;
            }
         }
      }
      return retVal;
   }

   /*++
   * @method: facebook::oAuthAccessToken
   *
   * @description: method to exchange request token with access token
   *
   * @input: none
   *
   * @output: true if everything went sucessfully, otherwise false
   *
   *--*/
   bool facebook::oAuthAccessToken()
   {
      bool retVal = false;

      if (true)
      {
         /* Get OAuth header for access token */
         property_set set(get_app());
         if (m_oauth.getOAuthHeader(eOAuthHttpGet, m_oauth.OAUTHLIB_TWITTER_ACCESS_TOKEN_URL, set, true))
         {
            if (performGet(m_oauth.OAUTHLIB_TWITTER_ACCESS_TOKEN_URL, set["headers"].propset()))
            {
               /* Tell OAuth object to save access token and secret from web response */
               string twitterResp("");
               twitterResp = get_response();
               m_oauth.extractOAuthTokenKeySecret(twitterResp);

               retVal = true;
            }
         }
      }
      return retVal;
   }


} // namespace hi5




