// Bij page reload alle ajax requests cancellen, zodat de pagina een beetje snel verversen kan.
var msgs = [];
window.addEventListener('beforeunload', ()=>{
    clearInterval(interval);
    msgs.forEach((req)=>{
        req.abort();
    });
});

function msg(txt, hook) {
    msgs.push($.ajax('command.php', {
        dataType: 'json',
        data: {
            id: 'x',            // Bewaker GUI ID
            msg:txt,
        },
        error: (data) => {
            console.log(data);
        }
    }).done((data)=>{hook(data)}));
}

$(document).ready(()=>{

    $('.message .close').on('click', ()=>{
        $(this).closest('.message').removeClass('message').addClass('hidden message');
    });

    $('.schemerlamp .lamp .checkbox').checkbox();

    $('.stoel .trillen .checkbox').checkbox();

    $('.zuil .zoemer .checkbox').checkbox({
        onChecked:()=>{msg('fza',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('fzu',(data)=>{console.log(data)})}
    });

    $('.zuil .brandalarm .checkbox').checkbox({
        onChecked:()=>{msg('fba',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('fbu',(data)=>{console.log(data)})}
    });

    $('.zuil .noodalarm .checkbox').checkbox({
        onChecked:()=>{msg('fna',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('fnu',(data)=>{console.log(data)})}
    });

    $('.koelkast .deur .checkbox').checkbox();
    $('.koelkast .koeler .checkbox').checkbox();

    $('.deur .inside.switch .checkbox').checkbox();

    $('.deur .outside.switch .checkbox').checkbox();
    $('.deur .inside.lamp .checkbox').checkbox();
    $('.deur .outside.lamp .checkbox').checkbox();
    $('.deur .deur .checkbox').checkbox({
        onChecked:()=>{msg('do',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('ds',(data)=>{console.log(data)})}
    });

    $('.bed .alarm .checkbox').checkbox();
    $('.bed .lamp .checkbox').checkbox({
        onChange:()=>{}
    });

    $('.muur .lamp .checkbox').checkbox();

    /* Update interval */
    interval = setInterval(function() {
        msg('-dsf', updateElements);
    }, 100);

    function updateElements(data) {

        /* SCHEMERLAMP */
        if (data.Schemerlamp) {
            let s = data.Schemerlamp, c = 'set checked';
            if (!s.Lamp) {
                c = 'set unchecked';
            }
            $('.schemerlamp .lamp .checkbox').checkbox(c);
            $('.schemerlamp .movement #bewegingssensor').text(s.Beweging);
        }

        /* ZUIL */
        if (data.Zuil) {
            let z = data.Zuil, c = 'set checked';
            if (!z.Zoemer) c = 'set unchecked';
            $('.zuil .zoemer .checkbox').checkbox(c);
            if (!z.Lamp) c = 'set unchecked'; else c = 'set checked';
            $('.zuil .lamp .checkbox').checkbox(c);
            if (!z.Brandalarm) c = 'set unchecked'; else c = 'set checked';
            $('.zuil .brandalarm .checkbox').checkbox(c);
            if (!z.Noodalarm) c = 'set unchecked'; else c = 'set checked';
            $('.zuil .noodalarm .checkbox').checkbox(c);

            /* BRANDALARM */
            if (z.Brandalarm) {
                $('.zuil .brand.message').show();
            } else {
                $('.zuil .brand.message').hide();
            }

            if (z.Noodalarm) {
                $('.zuil .nood.message').show();
            } else {
                $('.zuil .nood.message').hide();
            }
            $('.zuil .gasmelder-waarde').text(z.Gasmeter);
        }

        /* DEUR */
        if (data.Deur) {
            let buitenKnop = '.outside.switch', binnenKnop = '.inside.switch', buitenLed = '.outside.lamp', binnenLed = '.inside.lamp', deur = '.deur';
            let d = data.Deur, c = 'set checked';

            if (!d.Binnenknop) c = 'set unchecked';
            setCheckbox('.deur '+binnenKnop, c);

            if(d.Buitenknop) c = 'set checked';
            else c = 'set unchecked';
            setCheckbox('.deur '+buitenKnop, c);

            if (d.Binnenled) c = 'set checked';
            else c = 'set unchecked';
            setCheckbox('.deur '+binnenLed, c);

            if (d.Buitenled) c = 'set checked';
            else c = 'set unchecked';
            setCheckbox('.deur '+buitenLed, c);

            if (d.Deur == 'open') c = 'set checked';
            else c = 'set unchecked';
            setCheckbox('.deur '+deur, c);
        }
    }

    function setCheckbox(where, action) {
        $(where+" .checkbox").checkbox(action);
    }


    // function request(name, value, button) {
    //     button.checkbox('set disabled');
    //     $.ajax('command.php', {
    //         dataType: 'json',        /* Moet text zijn indien text */
    //         data: {
    //             name: name,
    //             val: value,
    //         },
    //         success: function() {
    //             button.checkbox('set enabled');
    //         },
    //         error: function(obj, reason, err) {
    //             console.log(reason);
    //             console.log(err);
    //             if (button.checkbox('is checked'))
    //                 button.checkbox('set unchecked');
    //             else
    //                 button.checkbox('set checked');
    //             button.checkbox('set enabled');
    //         }
    //     });
    // }

    // $('.koelkast .deur .ui.checkbox').checkbox();

    // let items = {
    //     koelkast: [
    //         'koeler'
    //     ],
    //     bed: [
    //         'lamp',
    //         'alarm'
    //     ],
    //     deur: [
    //         'outside.lamp',
    //         'inside.lamp',
    //         'outside.switch',
    //         'inside.switch'
    //     ],
    //     muur: [
    //         'lamp',
    //     ]
    // };

    // Object.keys(items).forEach(function(meubel) {
    //     items[meubel].forEach(function(apparaat)) {
    //         $('.'+meubel+' .'+ apparaat +' .ui.checkbox').checkbox({
    //             onChecked: function() {
    //                 request(meubel, apparaat+'=true', $(this));
    //             },
    //             onUnchecked: function() {
    //                 request(meubel, apparaat+'=false', $(this));
    //             }
    //         });
    //     }
    // });

});
