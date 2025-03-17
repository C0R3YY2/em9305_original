

document.addEventListener("DOMContentLoaded", function(){
  function toggle(e){
  if(e.style.display == 'none') show(e);
  else hide(e);
  }

  function show(e){
    if(e.dataset.displaywas === undefined){
      if(e.style.display == 'none')
        e.style.display = 'block'
      e.dataset.displaywas = e.style.display
    }
    e.style.display = e.dataset.displaywas
  }

  function hide(e){
        if(e.style.display != 'none')
          e.dataset.displaywas = e.style.display
        e.style.display = 'none'
  }

  // define search fields
  const search_field = document.querySelector('input.search')

  // search in thoses theses elements
  const filter_elements_selector = "section, div#index ul li"

  // search in thoses sub nodes
  const sub_filter = "a, dd p, td p, span"

  const load = document.querySelector('#search-loader')

  const toc = document.querySelector('#index.local.topic')

  search_field.addEventListener("input", (event) => {
    show(load)

    // search in the parent of the search field !
    found = search_filter(search_field.parentNode, event.target.value)

    hide(load)    
  });

  function search_filter(container, value){

    // get the searchable and filterable items
    elems = container.querySelectorAll(filter_elements_selector);

    divs = [...elems].filter(e => {
      
      search_target = e

      // split by newline and trim the content
      search_in = search_target.innerText.split('\n')[0].trim();

      // search command
      if(value[0] == ':'){
        _split = value.substring(1).split(' ')

        if(_split[0].length > 0 && e.querySelector(_split[0]) != null){
          search_target = e.querySelector(_split[0]);
      
          search_in = search_target.innerText.trim();
        }

        if(_split.length > 1 && _split[1].length > 1){
          value = _split[1]
        }else{
          return true
        }

      }

      //get value of the element
      // value return only the 1st level innerText
      if(search_target.innerText){
        search_in = ''
        
        c = search_target.querySelectorAll(sub_filter);
        c.forEach(function(e){
          search_in += e.textContent
        });
      }

      // show or hide element
      if(search_in.includes(value)) show(e);
      else hide(e);
      
      // filter return
      return e.style.display != 'none'
    });

    return divs.length

  }
});