if (Drupal.jsEnabled) {
  $(document).ready(function () {
    
    //theme log
    $('span.thmr_call').each(function () {
      var text = $(this).attr('thmr_key');
      var enabled = 1;
      var label = $('<span style="display: none" class="field-label field-tip">'+ text +'</span>').prependTo(this);
  
      $(this)
        .mouseover(function () { if (enabled) { $(label).show(); } })
        .mouseout(function () { if (enabled) { $(label).hide(); } });
    });
  });
}
