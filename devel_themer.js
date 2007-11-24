if (Drupal.jsEnabled) {
  $(document).ready(function () {
    lastObj = false;
    origBg = null;
    $('span.thmr_call')
    .each(function () {
      // make spans around block elements into block elements themselves
      var kids = $(this).children();
      for(i=0;i<kids.length;i++) {
        //console.log(kids[i].style.display);
        if (kids[i].clientHeight) {
          $(this).css('display', 'block');
          break;
        }
      }
    })
    .hover(function () {
      if (themerEnabled && this.parentNode.nodeName != 'BODY' && $(this).attr('thmr_curr') != 1) {
        $(this).css('outline', 'red solid 1px');
      }
    },
    function () {
      if (themerEnabled && $(this).attr('thmr_curr') != 1) {
        $(this).css('outline', 'none');
      }
    });

    var themerEnabled = 0;
    var themerToggle = function () {
      themerEnabled = 1 - themerEnabled;
      $('input', this).attr('checked', themerEnabled ? 'checked' : '');
      $('#themer-popup').css('display', themerEnabled ? 'block' : 'none');
      if (themerEnabled) {
        document.onclick = themerEvent;
        if (lastObj != false) {
          $(lastObj).css('outline', '3px solid #999');
        }
      }
      else {
        document.onclick = null;
        if (lastObj != false) {
          $(lastObj).css('outline', 'none');
        }
      }
    };
    $(Drupal.settings.thmr_popup)
      .appendTo($('body'));

    $('<div id="themer-toggle"><input type="checkbox" />Themer Info</div>')
      .appendTo($('body'))
      .click(themerToggle);

    $('#themer-popup').draggable({
      opacity: .6,
          handle: $('#themer-popup .topper')
    });
    // close box
    $('#themer-popup .topper .close').click(function() {
      themerToggle();
    });
  });
}

function themerHilight(obj) {
  // hilight the current object (and un-highlight the last)
  if (lastObj != false) {
    $(lastObj).css('outline', 'none').attr('thmr_curr', 0);
  }
  $(obj).css('outline', '#999 solid 3px').attr('thmr_curr', 1);
  lastObj = obj;
}

function themerDoIt(obj) {
  //console.log(obj);
  if (thmrInPop(obj)) {
    return true;
  }
  var objs = thmrFindParents(obj);
  if (objs.length) {
    themerHilight(objs[0]);
    thmrRebuildPopup(objs);
  }
  return false;
}

function thmrInPop(obj) {
  //is the element in either the popup box or the toggle div?
  if (obj.id == "themer-popup" || obj.id == "themer-toggle") return true;
  if (obj.parentNode) {
    while (obj = obj.parentNode) {
      if (obj.id=="themer-popup" || obj.id == "themer-toggle") return true;
    }
  }
  return false;
}

function themerEvent(e) {
  if (!e) {
    var e = window.event;
  };
  if (e.target) var tg = e.target;
  else if (e.srcElement) var tg = e.srcElement;
  return themerDoIt(tg);
}

/**
 * Find all parents with class="thmr_call"
 */
function thmrFindParents(obj) {
  var parents = new Array();
  if (obj.className == 'thmr_call') {
    parents[parents.length] = obj;
  }
  if (obj && obj.parentNode) {
    while (obj = obj.parentNode) {
      if (obj.className == 'thmr_call') {
        parents[parents.length] = obj;
      }
    }
  }
  return parents;
}

function thmrRefreshCollapse() {
  $('#themer-popup .devel-obj-output dt').each(function() {
      $(this).toggle(function() {
            $(this).parent().children('dd').show();
          }, function() {
            $(this).parent().children('dd').hide();
          });
    });
  $('#themer-popup .devel-obj-output dd').hide();
}

function thmrRebuildPopup(objs) {
  // rebuild the popup box
  var id = objs[0].id;
  var type = $(objs[0]).attr('thmr_type');
  var key = $(objs[0]).attr('thmr_key');
  var vars = Drupal.settings[id];
  var strs = Drupal.settings.thmrStrings;
  //console.log(vars);

  $('#themer-popup div.starter').empty();

  if (type == 'func') {
    $('#themer-popup dd.key').empty().prepend('<a href="'+ strs.api_site +'api/search/'+ strs.drupal_version +'/'+ key +'" title="'+ strs.drupal_api_docs +'">'+ key +'()</a>');
    $('#themer-popup dt.key-type').empty().prepend(strs.function_called);
  }
  else {
    $('#themer-popup dd.key').empty().prepend(key);
    $('#themer-popup dt.key-type').empty().prepend(strs.template_called);
  }

  // parents
  var parents = '';
  parents = strs.parents +' <span class="parents">';
  for(i=1;i<objs.length;i++) {
    parents += i!=1 ? '&lt; ' : '';
    parents += '<span class="parent" trig="'+ objs[i].id +'">'+ $(objs[i]).attr('thmr_key') +'</span> ';
  }
  parents += '</span>';

  $('#themer-popup #parents').empty().prepend(parents);
  $('#themer-popup span.parent').click(function() {
    $('#'+ $(this).attr('trig')).each(function() { themerDoIt(this) });
  })
  .hover(function() {
    $('#'+ $(this).attr('trig')).trigger('mouseover');
  },
  function() {
    $('#'+ $(this).attr('trig')).trigger('mouseout');
  });

  if (vars == undefined) {
    $('#themer-popup dd.candidates').empty();
    $('#themer-popup div.attributes').empty();
    $('#themer-popup div.used').empty();
  }
  else {
    if (type == 'func') {
      $('#themer-popup dt.candidates-type').empty().prepend(strs.candidate_functions);
      $('#themer-popup dd.candidates').empty().prepend(vars.candidates);

      $('#themer-popup div.attributes').empty().prepend('<h4>'+ strs.function_arguments + '</h4>' + vars.args);
      $('#themer-popup div.used').empty();
    }
    else {
      $('#themer-popup dt.candidates-type').empty().prepend(strs.candidate_files);
      $('#themer-popup dd.candidates').empty().prepend(vars.candidates.join(", "));

      $('#themer-popup div.attributes').empty().prepend('<h4>'+ strs.template_variables + '</h4>' + vars.arguments);
      $('#themer-popup div.used').empty().prepend('<dt>'+ strs.file_used  +'</dt><dd>'+ vars.used +'</dd>');
    }
    thmrRefreshCollapse();
  }
}