
var CLASSLISTS_MAIN_CONTENT_ID = 'maincontent-design';

function classListsFilter( baseURL, imgLoader )
{
    if ( !YAHOO.util.Connect.asyncRequest )
    {
        return true;
    }
    var callback = { success: function( o )
                              {
                                  var content = document.getElementById( CLASSLISTS_MAIN_CONTENT_ID );
                                  if ( !content )
                                  {
                                      return;
                                  }
                                  content.innerHTML = o.responseText;
                              }
                   };
    var classListsURL = classListsBuildURL( baseURL );
    classListsWait( imgLoader );
    var transaction = YAHOO.util.Connect.asyncRequest( 'GET', classListsURL, callback );
    return false;
}

function classListsWait( imgLoader )
{
    var content = document.getElementById( CLASSLISTS_MAIN_CONTENT_ID );
    if ( !content )
    {
        return;
    }
    content.innerHTML = '<p style="text-align:center;padding-top:4em;"><img src="' + imgLoader + '" alt="Loading..." /></p>';
}

function classListsBuildURL( baseURL )
{
    var classIdentfier = classListsGetSelectValue( 'classIdentifier' );
    var sortMethod = classListsGetSelectValue( 'sortMethod' );
    var sortOrder  = classListsGetSelectValue( 'sortOrder' );
    return baseURL + '/' + classIdentfier + '/' + sortMethod + '/' + sortOrder + '/ajax';

}

function classListsGetSelectValue( selectID )
{
    var element = document.getElementById( selectID );
    if ( !element )
    {
        return '';
    }
    return element.options[element.selectedIndex].value;
}
