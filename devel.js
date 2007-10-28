// $Id$

    
/**
  *  @name    jQuery Logging plugin
  *  @author  Dominic Mitchell
  *  @url     http://happygiraffe.net/blog/archives/2007/09/26/jquery-logging
  */
jQuery.fn.log = function (msg) {
    console.log("%s: %o", msg, this);
    return this;
};
    
if (Drupal.jsEnabled) {
  $(document).ready(function () {
    
    //theme log
    $('span.devel_theme_log_call').each(function () {
      var text = $(this).attr('theme_call_key');
      var enabled = 1;
      var label = $('<span style="display: none" class="field-label field-tip">'+ text +'</span>').prependTo(this);
  
      $(this)
        .mouseover(function () { if (enabled) { $(label).show(); } })
        .mouseout(function () { if (enabled) { $(label).hide(); } });
    });
  });
}
